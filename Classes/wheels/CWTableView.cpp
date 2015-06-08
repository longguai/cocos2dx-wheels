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
        _indices = new (std::nothrow) std::set<ssize_t>();
        _vordering = VerticalFillOrder::BOTTOM_UP;
        this->setDirection(Direction::VERTICAL);

        return true;
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

        if (_scrollBarEventCallback)
        {
            _scrollBarEventCallback(this, ScrollBarEvent::REFRESH_LENGTH);
            _scrollBarEventCallback(this, ScrollBarEvent::REFRESH_OFFSET);
        }
    }

    void TableView::setTableViewCellCallback(const ccTableViewCellCallback& callback)
    {
        _tableViewCellCallback = callback;
    }

    const TableView::ccTableViewCellCallback& TableView::getTableViewCellCallback() const
    {
        return _tableViewCellCallback;
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

    void TableView::setScrollBarEventCallback(const ccScrollBarEventCallback &func)
    {
        _scrollBarEventCallback = func;
    }

    const TableView::ccScrollBarEventCallback &TableView::getScrollBarEventCallback() const
    {
        return _scrollBarEventCallback;
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
            CCAssert(0, "TableView doesn't support both diretion!");
            break;
        default:
            CCAssert(0, "Unknown diretion!");
            break;
        }
    }

    const Vec2 &TableView::getInnerContainerOffset() const
    {
        return _innerContainer->getPosition();
    }

    void TableView::setInnerContainerOffset(const Vec2 &offset)
    {
        const Vec2 minOffset = this->minContainerOffset();
        const Vec2 maxOffset = this->maxContainerOffset();

        Vec2 o(MAX(minOffset.x, MIN(maxOffset.x, offset.x)), MAX(minOffset.y, MIN(maxOffset.y, offset.y)));
        _innerContainer->setPosition(o);
        ssize_t cellsCount = _numberOfCellsInTableView(this);
        _scrollViewDidScroll(cellsCount);
        this->scrollingEvent();

        if (_scrollBarEventCallback)
        {
            _scrollBarEventCallback(this, ScrollBarEvent::REFRESH_OFFSET);
        }
    }

    void TableView::reloadData()
    {
        _oldDirection = Direction::NONE;

        for (const auto &cell : _cellsUsed)
        {
            if (_tableViewCellCallback)
            {
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

        ssize_t cellsCount = _numberOfCellsInTableView(this);
        this->_updateCellPositions(cellsCount);
        this->_updateContentSize(cellsCount);
        if (cellsCount > 0)
        {
            _scrollViewDidScroll(cellsCount);
            this->scrollingEvent();
        }

        if (_scrollBarEventCallback)
        {
            _scrollBarEventCallback(this, ScrollBarEvent::REFRESH_LENGTH);
            _scrollBarEventCallback(this, ScrollBarEvent::REFRESH_OFFSET);
        }
    }

    void TableView::inplaceReloadData()
    {
        _oldDirection = _direction;

        for (const auto &cell : _cellsUsed)
        {
            if (_tableViewCellCallback)
            {
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

        ssize_t cellsCount = _numberOfCellsInTableView(this);
        this->_updateCellPositions(cellsCount);
        this->_updateContentSize(cellsCount);
        if (cellsCount > 0)
        {
            _scrollViewDidScroll(cellsCount);
            this->scrollingEvent();
        }

        if (_scrollBarEventCallback)
        {
            _scrollBarEventCallback(this, ScrollBarEvent::REFRESH_LENGTH);
            _scrollBarEventCallback(this, ScrollBarEvent::REFRESH_OFFSET);
        }
    }

    TableViewCell *TableView::cellAtIndex(ssize_t idx)
    {
        if (_indices->find(idx) != _indices->end())
        {
            for (const auto &cell : _cellsUsed)
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
        if (cell != nullptr)
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
        if (cell != nullptr)
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

        this->_updateCellPositions(countOfItems);
        this->_updateContentSize(countOfItems);
    }

    void TableView::removeCellAtIndex(ssize_t idx)
    {
        if (idx == CC_INVALID_INDEX)
        {
            return;
        }

        ssize_t uCountOfItems = _numberOfCellsInTableView(this);
        if (0 == uCountOfItems || idx + 1 > uCountOfItems)
        {
            return;
        }

        TableViewCell* cell = this->cellAtIndex(idx);
        if (cell == nullptr)
        {
            return;
        }

        ssize_t newIdx = _cellsUsed.getIndex(cell);

        //remove first
        this->_moveCellOutOfSight(cell);

        _indices->erase(idx);
        this->_updateCellPositions(uCountOfItems);

        for (ssize_t i = _cellsUsed.size() - 1; i > newIdx; --i)
        {
            cell = _cellsUsed.at(i);
            this->_setIndexForCell(cell->getIdx() - 1, cell);
        }
    }

    TableViewCell *TableView::dequeueCell()
    {
        TableViewCell *cell;
        if (_cellsFreed.empty())
        {
            cell = nullptr;
        }
        else
        {
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

    void TableView::_updateContentSize(ssize_t cellsCount)
    {
        Size size = Size::ZERO;
        if (cellsCount > 0)
        {
            float maxPosition = _cellsPositions[cellsCount];
            size = _contentSize;
            (_direction == Direction::HORIZONTAL ? size.width : size.height) = maxPosition;
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
        (_direction == Direction::HORIZONTAL ? offset.x : offset.y) = _cellsPositions[index];
        return offset;
    }

    long TableView::_indexFromOffset(Vec2 offset, ssize_t cellsCount)
    {
        long index = 0;
        const long maxIdx = cellsCount - 1;

        if (_vordering == VerticalFillOrder::TOP_DOWN)
        {
            offset.y = _innerContainer->getContentSize().height - offset.y;
        }
        index = this->__indexFromOffset(offset, cellsCount);
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

    long TableView::__indexFromOffset(const Vec2 &offset, ssize_t cellsCount)
    {
        long low = 0;
        long high = cellsCount - 1;
        const float search = (_direction == Direction::HORIZONTAL ? offset.x : offset.y);

        // binary search
        while (high >= low)
        {
            long index = low + (high - low) / 2;
            const float cellStart = _cellsPositions[index];
            const float cellEnd = _cellsPositions[index + 1];

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
        return (low <= 0) ? 0 : -1;
    }

    void TableView::_moveCellOutOfSight(TableViewCell *cell)
    {
        if (_tableViewCellCallback)
        {
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

    void TableView::_updateCellPositions(ssize_t cellsCount)
    {
        _cellsPositions.resize(cellsCount + 1, 0.0f);

        if (cellsCount > 0)
        {
            float currentPos = 0;
            Size cellSize;
            for (ssize_t i = 0; i < cellsCount; ++i)
            {
                _cellsPositions[i] = currentPos;
                cellSize = _tableCellSizeForIndex(this, i);
                currentPos += (_direction == Direction::HORIZONTAL ? cellSize.width : cellSize.height);
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

    void TableView::_scrollViewDidScroll(ssize_t cellsCount)
    {
        if (0 == cellsCount)
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
        offset.x = std::min(maxOffset.x, offset.x);
        offset.x = std::max(minOffset.x, offset.x);
        offset.y = std::min(maxOffset.y, offset.y);
        offset.y = std::max(minOffset.y, offset.y);

        offset.x = -offset.x;
        offset.y = -offset.y;
        maxIdx = std::max((long)cellsCount - 1, 0L);

        if (_vordering == VerticalFillOrder::TOP_DOWN)
        {
            offset.y = offset.y + _contentSize.height / _innerContainer->getScaleY();
        }
        startIdx = this->_indexFromOffset(offset, cellsCount);
        if (startIdx == CC_INVALID_INDEX)
        {
            startIdx = cellsCount - 1;
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

        endIdx = this->_indexFromOffset(offset, cellsCount);
        if (endIdx == CC_INVALID_INDEX)
        {
            endIdx = cellsCount - 1;
        }

#if 0 // For Testing.
        int i = 0;
        std::for_each(_cellsUsed.begin(), _cellsUsed.end(), [&i](TableViewCell* pCell) {
            log("cells Used index %d, value = %lu", i, pCell->getIdx());
            ++i;
        });
        log("---------------------------------------");
        i = 0;
        std::for_each(_cellsFreed.begin(), _cellsFreed.end(), [&i](TableViewCell* pCell) {
            log("cells freed index %d, value = %lu", i, pCell->getIdx());
            ++i;
        });
        log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

        if (!_cellsUsed.empty())
        {
            auto cell = _cellsUsed.at(0);
            idx = cell->getIdx();

            while (idx < startIdx)
            {
                this->_moveCellOutOfSight(cell);
                CC_BREAK_IF(_cellsUsed.empty());
                cell = _cellsUsed.at(0);
                idx = cell->getIdx();
            }
        }
        if (!_cellsUsed.empty())
        {
            auto cell = _cellsUsed.back();
            idx = cell->getIdx();

            while (idx <= maxIdx && idx > endIdx)
            {
                this->_moveCellOutOfSight(cell);
                CC_BREAK_IF(_cellsUsed.empty());
                cell = _cellsUsed.back();
                idx = cell->getIdx();
            }
        }

        for (ssize_t i = startIdx; i <= endIdx; ++i)
        {
            if (_indices->find(i) == _indices->end())
            {
                this->updateCellAtIndex(i);
            }
        }
    }

    void TableView::onTouchEnded(Touch *pTouch, Event *pEvent)
    {
        if (!this->isVisible())
        {
            return;
        }

        if (_touchedCell != nullptr)
        {
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

        Vec2 point = this->_innerContainer->convertTouchToNodeSpace(pTouch);
        ssize_t cellsCount = _numberOfCellsInTableView(this);
        long index = this->_indexFromOffset(point, cellsCount);
        _touchedCell = (index == CC_INVALID_INDEX ? nullptr :  this->cellAtIndex(index));

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

        if (_touchedCell != nullptr)
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
        ssize_t cellsCount = _numberOfCellsInTableView(this);
        _scrollViewDidScroll(cellsCount);
        if (_scrollBarEventCallback)
        {
            _scrollBarEventCallback(this, ScrollBarEvent::REFRESH_OFFSET);
        }
        return ret;
    }
}
