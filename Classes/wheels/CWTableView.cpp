#include "CWTableView.h"

using namespace cocos2d;
using namespace cocos2d::ui;

namespace cw {
    TableView::TableView()
    : _touchedCell(nullptr)
    , _vordering(VerticalFillOrder::BOTTOM_UP)
    , _indices(nullptr)
    , _tableViewCellCallback()
    , _tableCellSizeForIndex(TableView::__tableCellSizeForIndex)
    , _tableCellAtIndex(TableView::__tableCellAtIndex)
    , _numberOfCellsInTableView(TableView::__numberOfCellsInTableView)
    , _oldDirection(Direction::NONE)
    , _isUsedCellsDirty(false)
    {

    }

    TableView::~TableView()
    {
        CC_SAFE_DELETE(_indices);
    }

    TableView *TableView::create(TableViewDataSource *dataSource, const Size &size)
    {
        TableView *table = new (std::nothrow) TableView();
        if (table != nullptr && table->initWithViewSize(dataSource, size))
        {
            table->autorelease();
            return table;
        }
        CC_SAFE_DELETE(table);
        return NULL;
    }

    TableView *TableView::create()
    {
        TableView *table = new (std::nothrow) TableView();
        if (table != nullptr && table->init())
        {
            table->autorelease();
            return table;
        }
        CC_SAFE_DELETE(table);
        return NULL;
    }

    bool TableView::initWithViewSize(TableViewDataSource *dataSource, const Size &size)
    {
        if (ScrollView::init())
        {
            _oldDirection = Direction::NONE;
            _cellsPositions.clear();

            _initWithViewSize(size);
            setDataSource(dataSource);

            _updateCellPositions();
            _updateContentSize();

            return true;
        }
        return false;
    }

    bool TableView::init()
    {
        if (!ScrollView::init())
        {
            return false;
        }
        _oldDirection = Direction::NONE;
        _cellsPositions.clear();

        _cellsUsed.clear();
        _cellsFreed.clear();
        CC_SAFE_DELETE(_indices);
        _indices = new std::set<ssize_t>();
        _vordering = VerticalFillOrder::BOTTOM_UP;
        this->setDirection(Direction::VERTICAL);

        return true;
    }

    void TableView::_initWithViewSize(const Size &size)
    {
        this->setContentSize(size);

        _cellsUsed.clear();
        _cellsFreed.clear();
        CC_SAFE_DELETE(_indices);
        _indices = new std::set<ssize_t>();
        _vordering = VerticalFillOrder::BOTTOM_UP;
        this->setDirection(Direction::VERTICAL);
    }

    void TableView::onSizeChanged()
    {
        Size orginSize = _innerContainer->getContentSize();
        Vec2 orginPos = _innerContainer->getPosition();
        ScrollView::onSizeChanged();
        if (orginSize.equals(_innerContainer->getContentSize()))
        {
            _innerContainer->setPosition(orginPos);
        }
    }

    void TableView::setDataSource(TableViewDataSource* source)
    {
        _tableCellSizeForIndex = std::bind(&TableViewDataSource::tableCellSizeForIndex, source, std::placeholders::_1, std::placeholders::_2);
        _tableCellAtIndex = std::bind(&TableViewDataSource::tableCellAtIndex, source, std::placeholders::_1, std::placeholders::_2);
        _numberOfCellsInTableView = std::bind(&TableViewDataSource::numberOfCellsInTableView, source, std::placeholders::_1);
    }

    void TableView::setDelegate(TableViewDelegate* pDelegate)
    {
        _tableViewCellCallback = [pDelegate](TableView* table, TableViewCell* cell, CellEventType type) {
            switch (type)
            {
            case CellEventType::TOUCHED:
                pDelegate->tableCellTouched(table, cell);
                break;
            case CellEventType::HIGHLIGHT:
                pDelegate->tableCellHighlight(table, cell);
                break;
            case CellEventType::UNHIGHLIGHT:
                pDelegate->tableCellUnhighlight(table, cell);
                break;
            case CellEventType::WILL_RECYCLE:
                pDelegate->tableCellWillRecycle(table, cell);
                break;
            default:
                break;
            }
        };
    }

    void TableView::setTableViewCellCallback(const ccTableViewCellCallback& callback)
    {
        _tableViewCellCallback = callback;
    }

    const TableView::ccTableViewCellCallback& TableView::getTableViewCellCallback() const
    {
        return _tableViewCellCallback;
    }

    void TableView::setDataSourceCallback(const ccDataSourceCallback& callback)
    {
        if (callback != nullptr)
        {
            _tableCellSizeForIndex = [callback](TableView *table, ssize_t idx) {
                Size size;
                callback(DataSourceFunction::CELL_SIZE_FOR_INDEX, table, idx, (intptr_t)&size);
                return size;
            };
            _tableCellAtIndex = [callback](TableView *table, ssize_t idx) {
                return (TableViewCell *)callback(DataSourceFunction::CELL_AT_INDEX, table, idx, 0);
            };
            _numberOfCellsInTableView = [callback](TableView *table) {
                return (ssize_t)callback(DataSourceFunction::NUMBERS_OF_CELLS, table, 0, 0);
            };
        }
        else
        {
            _tableCellSizeForIndex = TableView::__tableCellSizeForIndex;
            _tableCellAtIndex = TableView::__tableCellAtIndex;
            _numberOfCellsInTableView = TableView::__numberOfCellsInTableView;
        }
    }

    void TableView::setTableCellSizeForIndexCallback(const ccTableCellSizeForIndexCallback& func)
    {
        _tableCellSizeForIndex = func ? func : TableView::__tableCellSizeForIndex;
    }

    const TableView::ccTableCellSizeForIndexCallback& TableView::getTableCellSizeForIndexCallback() const
    {
        return _tableCellSizeForIndex;
    }

    void TableView::setTableCellAtIndexCallback(const ccTableCellAtIndexCallback& func)
    {
        _tableCellAtIndex = func ? func : TableView::__tableCellAtIndex;
    }

    const TableView::ccTableCellAtIndexCallback& TableView::getTableCellAtIndexCallback() const
    {
        return _tableCellAtIndex;
    }

    void TableView::setNumberOfCellsInTableViewCallback(const ccNumberOfCellsInTableViewCallback& func)
    {
        _numberOfCellsInTableView = func ? func : TableView::__numberOfCellsInTableView;
    }

    const TableView::ccNumberOfCellsInTableViewCallback& TableView::getNumberOfCellsInTableViewCallback() const
    {
        return _numberOfCellsInTableView;
    }

    void TableView::setVerticalFillOrder(VerticalFillOrder fillOrder)
    {
        if (_vordering != fillOrder)
        {
            _vordering = fillOrder;
            if (!_cellsUsed.empty())
            {
                this->reloadData();
            }
        }
    }

    TableView::VerticalFillOrder TableView::getVerticalFillOrder()
    {
        return _vordering;
    }

    void TableView::setDirection(Direction dir)
    {
        switch (_direction)
        {
            case Direction::NONE:
            case Direction::HORIZONTAL:
            case Direction::VERTICAL:
                _direction = dir;
                break;
            case Direction::BOTH:
                CCLOG("TableView doesn't support both diretion!");
                break;
            default:
                CCLOG("Unknown diretion!");
                break;
        }
    }

    Vec2 TableView::getInnerContainerOffset() const
    {
        return _innerContainer->getPosition();
    }

    void TableView::setInnerContainerOffset(const Vec2 &offset)
    {
        const Vec2 minOffset = this->minContainerOffset();
        const Vec2 maxOffset = this->maxContainerOffset();

        Vec2 o(MAX(minOffset.x, MIN(maxOffset.x, offset.x)), MAX(minOffset.y, MIN(maxOffset.y, offset.y)));
        _innerContainer->setPosition(o);
        _scrollViewDidScroll();
        this->scrollingEvent();
    }

    void TableView::reloadData()
    {
        _oldDirection = Direction::NONE;

        for(const auto &cell : _cellsUsed) {
            if (_tableViewCellCallback) {
                _tableViewCellCallback(this, cell, CellEventType::WILL_RECYCLE);
            }

            _cellsFreed.pushBack(cell);

            cell->reset();
            if (cell->getParent() == _innerContainer)
            {
                _innerContainer->removeChild(cell, true);
            }
        }

        _indices->clear();
        _cellsUsed.clear();

        this->_updateCellPositions();
        this->_updateContentSize();
        if (_numberOfCellsInTableView(this) > 0)
        {
            _scrollViewDidScroll();
            this->scrollingEvent();
        }
    }

    TableViewCell *TableView::cellAtIndex(ssize_t idx)
    {
        if (_indices->find(idx) != _indices->end())
        {
            for (const auto& cell : _cellsUsed)
            {
                if (cell->getIdx() == idx)
                {
                    return cell;
                }
            }
        }

        return nullptr;
    }

    void TableView::updateCellAtIndex(ssize_t idx)
    {
        if (idx == CC_INVALID_INDEX)
        {
            return;
        }
        ssize_t countOfItems = _numberOfCellsInTableView(this);
        if (0 == countOfItems || idx > countOfItems-1)
        {
            return;
        }

        TableViewCell* cell = this->cellAtIndex(idx);
        if (cell)
        {
            this->_moveCellOutOfSight(cell);
        }
        cell = _tableCellAtIndex(this, idx);
        this->_setIndexForCell(idx, cell);
        this->_addCellIfNecessary(cell);
    }

    void TableView::insertCellAtIndex(ssize_t idx)
    {
        if (idx == CC_INVALID_INDEX)
        {
            return;
        }

        ssize_t countOfItems = _numberOfCellsInTableView(this);
        if (0 == countOfItems || idx > countOfItems-1)
        {
            return;
        }

        long newIdx = 0;

        auto cell = cellAtIndex(idx);
        if (cell)
        {
            newIdx = _cellsUsed.getIndex(cell);
            // Move all cells behind the inserted position
            for (ssize_t i = newIdx; i < _cellsUsed.size(); ++i)
            {
                cell = _cellsUsed.at(i);
                this->_setIndexForCell(cell->getIdx()+1, cell);
            }
        }

        //insert a new cell
        cell = _tableCellAtIndex(this, idx);
        this->_setIndexForCell(idx, cell);
        this->_addCellIfNecessary(cell);

        this->_updateCellPositions();
        this->_updateContentSize();
    }

    void TableView::removeCellAtIndex(ssize_t idx)
    {
        if (idx == CC_INVALID_INDEX)
        {
            return;
        }

        long uCountOfItems = _numberOfCellsInTableView(this);
        if (0 == uCountOfItems || idx > uCountOfItems-1)
        {
            return;
        }

        ssize_t newIdx = 0;

        TableViewCell* cell = this->cellAtIndex(idx);
        if (cell == nullptr)
        {
            return;
        }

        newIdx = _cellsUsed.getIndex(cell);

        //remove first
        this->_moveCellOutOfSight(cell);

        _indices->erase(idx);
        this->_updateCellPositions();

        for (ssize_t i = _cellsUsed.size()-1; i > newIdx; --i)
        {
            cell = _cellsUsed.at(i);
            this->_setIndexForCell(cell->getIdx()-1, cell);
        }
    }

    TableViewCell *TableView::dequeueCell()
    {
        TableViewCell *cell;

        if (_cellsFreed.empty()) {
            cell = nullptr;
        } else {
            cell = _cellsFreed.at(0);
            cell->retain();
            _cellsFreed.erase(0);
            cell->autorelease();
        }
        return cell;
    }

    void TableView::_addCellIfNecessary(TableViewCell * cell)
    {
        if (cell->getParent() != _innerContainer)
        {
            _innerContainer->addChild(cell);
        }
        _cellsUsed.pushBack(cell);
        _indices->insert(cell->getIdx());
        _isUsedCellsDirty = true;
    }

    void TableView::_updateContentSize()
    {
        Size size = Size::ZERO;
        ssize_t cellsCount = _numberOfCellsInTableView(this);

        if (cellsCount > 0)
        {
            float maxPosition = _cellsPositions[cellsCount];

            switch (_direction)
            {
                case Direction::HORIZONTAL:
                size = Size(maxPosition, _contentSize.height);
                break;
                default:
                size = Size(_contentSize.width, maxPosition);
                break;
            }
        }

        this->setInnerContainerSize(size);

        if (_oldDirection != _direction)
        {
            if (_direction == Direction::HORIZONTAL)
            {
                _innerContainer->setPositionX(0.0f);
            }
            else
            {
                Vec2 minOffset = this->minContainerOffset();
                _innerContainer->setPositionY(minOffset.y);
            }
            _oldDirection = _direction;
        }

    }

    Vec2 TableView::_offsetFromIndex(ssize_t index)
    {
        Vec2 offset = this->__offsetFromIndex(index);

        const Size cellSize = _tableCellSizeForIndex(this, index);
        if (_vordering == VerticalFillOrder::TOP_DOWN)
        {
            offset.y = _innerContainer->getContentSize().height - offset.y - cellSize.height;
        }
        return offset;
    }

    Vec2 TableView::__offsetFromIndex(ssize_t index)
    {
        Vec2 offset;
        Size  cellSize;

        switch (_direction)
        {
            case Direction::HORIZONTAL:
            offset = Vec2(_cellsPositions[index], 0.0f);
            break;
            default:
            offset = Vec2(0.0f, _cellsPositions[index]);
            break;
        }

        return offset;
    }

    long TableView::_indexFromOffset(Vec2 offset)
    {
        long index = 0;
        const long maxIdx = _numberOfCellsInTableView(this) - 1;

        if (_vordering == VerticalFillOrder::TOP_DOWN)
        {
            offset.y = _innerContainer->getContentSize().height - offset.y;
        }
        index = this->__indexFromOffset(offset);
        if (index != -1)
        {
            index = MAX(0, index);
            if (index > maxIdx)
            {
                index = CC_INVALID_INDEX;
            }
        }

        return index;
    }

    long TableView::__indexFromOffset(Vec2 offset)
    {
        long low = 0;
        long high = _numberOfCellsInTableView(this) - 1;
        float search;
        switch (_direction)
        {
            case Direction::HORIZONTAL:
            search = offset.x;
            break;
            default:
            search = offset.y;
            break;
        }

        while (high >= low)
        {
            long index = low + (high - low) / 2;
            float cellStart = _cellsPositions[index];
            float cellEnd = _cellsPositions[index + 1];

            if (search >= cellStart && search <= cellEnd)
            {
                return index;
            }
            else if (search < cellStart)
            {
                high = index - 1;
            }
            else
            {
                low = index + 1;
            }
        }

        if (low <= 0) {
            return 0;
        }

        return -1;
    }

    void TableView::_moveCellOutOfSight(TableViewCell *cell)
    {
        if (_tableViewCellCallback) {
            _tableViewCellCallback(this, cell, CellEventType::WILL_RECYCLE);
        }

        _cellsFreed.pushBack(cell);
        _cellsUsed.eraseObject(cell);
        _isUsedCellsDirty = true;

        _indices->erase(cell->getIdx());
        cell->reset();

        if (cell->getParent() == _innerContainer)
        {
            _innerContainer->removeChild(cell, true);
        }
    }

    void TableView::_setIndexForCell(ssize_t index, TableViewCell *cell)
    {
        cell->setAnchorPoint(Vec2(0.0f, 0.0f));
        cell->setPosition(this->_offsetFromIndex(index));
        cell->setIdx(index);
    }

    void TableView::_updateCellPositions()
    {
        ssize_t cellsCount = _numberOfCellsInTableView(this);
        _cellsPositions.resize(cellsCount + 1, 0.0);

        if (cellsCount > 0)
        {
            float currentPos = 0;
            Size cellSize;
            for (ssize_t i=0; i < cellsCount; i++)
            {
                _cellsPositions[i] = currentPos;
                cellSize = _tableCellSizeForIndex(this, i);
                switch (_direction)
                {
                    case Direction::HORIZONTAL:
                    currentPos += cellSize.width;
                    break;
                    default:
                    currentPos += cellSize.height;
                    break;
                }
            }
            _cellsPositions[cellsCount] = currentPos;//1 extra value allows us to get right/bottom of the last cell
        }

    }

    Vec2 TableView::maxContainerOffset()
    {
        return Vec2(0, 0);
    }

    Vec2 TableView::minContainerOffset()
    {
        const Size &contentSize = _innerContainer->getContentSize();
        const Size &viewSize = _contentSize;
        return Vec2(viewSize.width - contentSize.width * _innerContainer->getScaleX(),
                    viewSize.height - contentSize.height * _innerContainer->getScaleY());
    }

    void TableView::_scrollViewDidScroll()
    {
        long countOfItems = _numberOfCellsInTableView(this);
        if (0 == countOfItems)
        {
            return;
        }

        if (_isUsedCellsDirty)
        {
            _isUsedCellsDirty = false;
            std::sort(_cellsUsed.begin(), _cellsUsed.end(), [](TableViewCell *a, TableViewCell *b) -> bool{
                return a->getIdx() < b->getIdx();
            });
        }

        ssize_t startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
        Vec2 offset = _innerContainer->getPosition();
        Vec2 maxOffset = maxContainerOffset();
        Vec2 minOffset = minContainerOffset();
        offset.x = MIN(maxOffset.x, offset.x);
        offset.x = MAX(minOffset.x, offset.x);
        offset.y = MIN(maxOffset.y, offset.y);
        offset.y = MAX(minOffset.y, offset.y);

        offset.x *= -1;
        offset.y *= -1;
        maxIdx = MAX(countOfItems-1, 0);

        if (_vordering == VerticalFillOrder::TOP_DOWN)
        {
            offset.y = offset.y + _contentSize.height / _innerContainer->getScaleY();
        }
        startIdx = this->_indexFromOffset(offset);
        if (startIdx == CC_INVALID_INDEX)
        {
            startIdx = countOfItems - 1;
        }

        if (_vordering == VerticalFillOrder::TOP_DOWN)
        {
            offset.y -= _contentSize.height / _innerContainer->getScaleY();
        }
        else
        {
            offset.y += _contentSize.height / _innerContainer->getScaleY();
        }
        offset.x += _contentSize.width / _innerContainer->getScaleX();

        endIdx   = this->_indexFromOffset(offset);
        if (endIdx == CC_INVALID_INDEX)
        {
            endIdx = countOfItems - 1;
        }

#if 0 // For Testing.
        int i = 0;
        std::for_each(_cellsUsed.begin(), _cellsUsed.end(), [&i](TableViewCell* pCell) {
            log("cells Used index %d, value = %lu", i, pCell->getIdx());
            i++;
        });
        log("---------------------------------------");
        i = 0;
        std::for_each(_cellsFreed.begin(), _cellsFreed.end(), [&i](TableViewCell* pCell) {
            log("cells freed index %d, value = %lu", i, pCell->getIdx());
            i++;
        });
        log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

        if (!_cellsUsed.empty())
        {
            auto cell = _cellsUsed.at(0);
            idx = cell->getIdx();

            while(idx < startIdx)
            {
                this->_moveCellOutOfSight(cell);
                if (!_cellsUsed.empty())
                {
                    cell = _cellsUsed.at(0);
                    idx = cell->getIdx();
                }
                else
                {
                    break;
                }
            }
        }
        if (!_cellsUsed.empty())
        {
            auto cell = _cellsUsed.back();
            idx = cell->getIdx();

            while(idx <= maxIdx && idx > endIdx)
            {
                this->_moveCellOutOfSight(cell);
                if (!_cellsUsed.empty())
                {
                    cell = _cellsUsed.back();
                    idx = cell->getIdx();
                }
                else
                {
                    break;
                }
            }
        }

        for (ssize_t i = startIdx; i <= endIdx; i++)
        {
            if (_indices->find(i) != _indices->end())
            {
                continue;
            }
            this->updateCellAtIndex(i);
        }
    }

    void TableView::onTouchEnded(Touch *pTouch, Event *pEvent)
    {
        if (!this->isVisible()) {
            return;
        }

        if (_touchedCell){
            Rect bb = this->getBoundingBox();
            bb.origin = _parent->convertToWorldSpace(bb.origin);

            if (bb.containsPoint(pTouch->getLocation()))
            {
                if (_tableViewCellCallback)
                {
                    _tableViewCellCallback(this, _touchedCell, CellEventType::UNHIGHLIGHT);
                    _tableViewCellCallback(this, _touchedCell, CellEventType::TOUCHED);
                }
            }

            _touchedCell = nullptr;
        }

        ScrollView::onTouchEnded(pTouch, pEvent);
    }

    bool TableView::onTouchBegan(Touch *pTouch, Event *pEvent)
    {
        if (!this->isVisible())
        {
            return false;
        }

        bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);

        long index;
        Vec2 point;

        point = this->_innerContainer->convertTouchToNodeSpace(pTouch);

        index = this->_indexFromOffset(point);
        if (index == CC_INVALID_INDEX)
        {
            _touchedCell = nullptr;
        }
        else
        {
            _touchedCell  = this->cellAtIndex(index);
        }

        if (_touchedCell != nullptr)
        {
            if (_tableViewCellCallback)
            {
                _tableViewCellCallback(this, _touchedCell, CellEventType::HIGHLIGHT);
            }
        }

        return touchResult;
    }

    void TableView::onTouchMoved(Touch *pTouch, Event *pEvent)
    {
        ScrollView::onTouchMoved(pTouch, pEvent);

        if (_touchedCell != nullptr)
        {
            if (_tableViewCellCallback)
            {
                _tableViewCellCallback(this, _touchedCell, CellEventType::UNHIGHLIGHT);
            }

            _touchedCell = nullptr;
        }
    }

    void TableView::onTouchCancelled(Touch *pTouch, Event *pEvent)
    {
        ScrollView::onTouchCancelled(pTouch, pEvent);

        if (_touchedCell)
        {
            if (_tableViewCellCallback)
            {
                _tableViewCellCallback(this, _touchedCell, CellEventType::UNHIGHLIGHT);
            }

            _touchedCell = nullptr;
        }
    }

    bool TableView::scrollChildren(float touchOffsetX, float touchOffsetY)
    {
        bool ret = ScrollView::scrollChildren(touchOffsetX, touchOffsetY);
        _scrollViewDidScroll();
        return ret;
    }
}
