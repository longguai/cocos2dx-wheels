#ifndef __UITABLEVIEW__
#define __UITABLEVIEW__

#include "ui/UIScrollView.h"
#include <set>
#include <vector>

namespace cw {
    /**
     * Abstract class for SWTableView cell node
     */
    class TableViewCell : public cocos2d::ui::Widget
    {
    public:
        CREATE_FUNC(TableViewCell);

        TableViewCell() {}
        /**
         * The index used internally by SWTableView and its subclasses
         */
        ssize_t getIdx() const { return _idx; }
        void setIdx(ssize_t uIdx) { _idx = uIdx; }
        /**
         * Cleans up any resources linked to this cell and resets <code>idx</code> property.
         */
        void reset() { _idx = cocos2d::CC_INVALID_INDEX; }

    private:
        ssize_t _idx;
    };

    class TableView;

    /**
     * Sole purpose of this delegate is to single touch event in this version.
     */
    class CC_DEPRECATED_ATTRIBUTE TableViewDelegate
    {
    public:
        virtual ~TableViewDelegate() { }

        /**
         * Delegate to respond touch event
         *
         * @param table table contains the given cell
         * @param cell  cell that is touched
         * @js NA
         * @lua NA
         */
        virtual void tableCellTouched(TableView* table, TableViewCell* cell) = 0;

        /**
         * Delegate to respond a table cell press event.
         *
         * @param table table contains the given cell
         * @param cell  cell that is pressed
         * @js NA
         * @lua NA
         */
        virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};

        /**
         * Delegate to respond a table cell release event
         *
         * @param table table contains the given cell
         * @param cell  cell that is pressed
         * @js NA
         * @lua NA
         */
        virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};

        /**
         * Delegate called when the cell is about to be recycled. Immediately
         * after this call the cell will be removed from the scene graph and
         * recycled.
         *
         * @param table table contains the given cell
         * @param cell  cell that is pressed
         * @js NA
         * @lua NA
         */
        virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    };

    /**
     * Data source that governs table backend data.
     */
    class CC_DEPRECATED_ATTRIBUTE TableViewDataSource
    {
    public:
        /**
         * @js NA
         * @lua NA
         */
        virtual ~TableViewDataSource() {}

        /**
         * cell size for a given index
         *
         * @param idx the index of a cell to get a size
         * @return size of a cell at given index
         */
        virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx) {
            return cellSizeForTable(table);
        };
        /**
         * cell height for a given table.
         *
         * @param table table to hold the instances of Class
         * @return cell size
         */
        virtual cocos2d::Size cellSizeForTable(TableView *table) {
            return cocos2d::Size::ZERO;
        };
        /**
         * a cell instance at a given index
         *
         * @param idx index to search for a cell
         * @return cell found at idx
         */
        virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) = 0;
        /**
         * Returns number of cells in a given table view.
         *
         * @return number of cells
         */
        virtual ssize_t numberOfCellsInTableView(TableView *table) = 0;

    };

    class TableView : public cocos2d::ui::ScrollView
    {
        DECLARE_CLASS_GUI_INFO

    public:
        enum class VerticalFillOrder
        {
            TOP_DOWN,
            BOTTOM_UP
        };

        enum class CellEventType
        {
            TOUCHED,
            HIGHLIGHT,
            UNHIGHLIGHT,
            WILL_RECYCLE
        };
        typedef std::function<void (TableView* table, TableViewCell* cell, CellEventType)> ccTableViewCellCallback;

        typedef std::function<cocos2d::Size (TableView *table, ssize_t idx)> ccTableCellSizeForIndexCallback;
        typedef std::function<TableViewCell *(TableView *table, ssize_t idx)> ccTableCellAtIndexCallback;
        typedef std::function<ssize_t (TableView *table)> ccNumberOfCellsInTableViewCallback;

        /**
         * Default constructor
         */
        TableView();

        /**
         * Default destructor
         */
        virtual ~TableView();

        /**
         * Allocates and initializes.
         */
        static TableView *create();
        CC_DEPRECATED_ATTRIBUTE static TableView *create(TableViewDataSource *dataSource, const cocos2d::Size &size);

        bool init();
        CC_DEPRECATED_ATTRIBUTE bool initWithViewSize(TableViewDataSource *dataSource, const cocos2d::Size &size);

        /**
         * data source
         * @js NA
         * @lua NA
         */
        CC_DEPRECATED_ATTRIBUTE TableViewDataSource* getDataSource() { return nullptr; }
        /**
         * when this function bound to js or lua,the input params are changed
         * in js:var setDataSource(var jsSource)
         * in lua:local setDataSource()
         * @endcode
         */
        CC_DEPRECATED_ATTRIBUTE void setDataSource(TableViewDataSource* source);
        /**
         * delegate
         * @js NA
         * @lua NA
         */
        CC_DEPRECATED_ATTRIBUTE TableViewDelegate* getDelegate() { return nullptr; }
        /**
         * @code
         * when this function bound to js or lua,the input params are changed
         * in js:var setDelegate(var jsDelegate)
         * in lua:local setDelegate()
         * @endcode
         */
        CC_DEPRECATED_ATTRIBUTE void setDelegate(TableViewDelegate* pDelegate);

        void setTableViewCellCallback(const ccTableViewCellCallback& callback);
        const ccTableViewCellCallback& getTableViewCellCallback() const;

        void setTableCellSizeForIndexCallback(const ccTableCellSizeForIndexCallback& func);
        const ccTableCellSizeForIndexCallback& getTableCellSizeForIndexCallback() const;
        void setTableCellAtIndexCallback(const ccTableCellAtIndexCallback& func);
        const ccTableCellAtIndexCallback& getTableCellAtIndexCallback() const;
        void setNumberOfCellsInTableViewCallback(const ccNumberOfCellsInTableViewCallback& func);
        const ccNumberOfCellsInTableViewCallback& getNumberOfCellsInTableViewCallback() const;

        /**
         * determines how cell is ordered and filled in the view.
         */
        void setVerticalFillOrder(VerticalFillOrder order);
        VerticalFillOrder getVerticalFillOrder();

        cocos2d::Vec2 getInnerContainerOffset() const;
        void setInnerContainerOffset(const cocos2d::Vec2 &offset);

        /**
         * Updates the content of the cell at a given index.
         *
         * @param idx index to find a cell
         */
        void updateCellAtIndex(ssize_t idx);
        /**
         * Inserts a new cell at a given index
         *
         * @param idx location to insert
         */
        void insertCellAtIndex(ssize_t idx);
        /**
         * Removes a cell at a given index
         *
         * @param idx index to find a cell
         */
        void removeCellAtIndex(ssize_t idx);
        /**
         * reloads data from data source.  the view will be refreshed.
         */
        void reloadData();
        void inplaceReloadData();

        /**
         * Dequeues a free cell if available. nil if not.
         *
         * @return free cell
         */
        TableViewCell *dequeueCell();

        /**
         * Returns an existing cell at a given index. Returns nil if a cell is nonexistent at the moment of query.
         *
         * @param idx index
         * @return a cell at a given index
         */
        TableViewCell *cellAtIndex(ssize_t idx);

        virtual void setDirection(cocos2d::ui::ScrollView::Direction dir) override;

        //handle touch event
        virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
        virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
        virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
        virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;

        cocos2d::Vec2 maxContainerOffset();
        cocos2d::Vec2 minContainerOffset();

    protected:
        static cocos2d::Size __tableCellSizeForIndex(TableView*, ssize_t) { return cocos2d::Size::ZERO; }
        static TableViewCell* __tableCellAtIndex(TableView*, ssize_t) { return nullptr; }
        static ssize_t __numberOfCellsInTableView(TableView*) { return 0; }

        CC_DEPRECATED_ATTRIBUTE void _initWithViewSize(const cocos2d::Size &size);

        virtual void onSizeChanged() override;

        void _scrollEvent(Ref *sender, cocos2d::ui::ScrollView::EventType type);

        virtual bool scrollChildren(float touchOffsetX, float touchOffsetY) override;

        long __indexFromOffset(const cocos2d::Vec2 &offset);
        long _indexFromOffset(cocos2d::Vec2 offset);
        cocos2d::Vec2 __offsetFromIndex(ssize_t index);
        cocos2d::Vec2 _offsetFromIndex(ssize_t index);

        void _moveCellOutOfSight(TableViewCell *cell);
        void _setIndexForCell(ssize_t index, TableViewCell *cell);
        void _addCellIfNecessary(TableViewCell * cell);

        void _updateCellPositions();
        void _scrollViewDidScroll();

        TableViewCell *_touchedCell;
        /**
         * vertical direction of cell filling
         */
        VerticalFillOrder _vordering;

        /**
         * index set to query the indexes of the cells used.
         */
        std::set<ssize_t>* _indices;

        /**
         * vector with all cell positions
         */
        std::vector<float> _cellsPositions;
        //NSMutableIndexSet *indices_;
        /**
         * cells that are currently in the table
         */
        cocos2d::Vector<TableViewCell*> _cellsUsed;
        /**
         * free list of cells
         */
        cocos2d::Vector<TableViewCell*> _cellsFreed;

        ccTableViewCellCallback _tableViewCellCallback;

        ccTableCellSizeForIndexCallback _tableCellSizeForIndex;
        ccTableCellAtIndexCallback _tableCellAtIndex;
        ccNumberOfCellsInTableViewCallback _numberOfCellsInTableView;

        cocos2d::ui::ScrollView::Direction _oldDirection;

        bool _isUsedCellsDirty;

    public:
        void _updateContentSize();

    private:
        using cocos2d::ui::ScrollView::setInnerContainerSize;
        using cocos2d::ui::ScrollView::addChild;
        using cocos2d::ui::ScrollView::removeAllChildren;
        using cocos2d::ui::ScrollView::removeAllChildrenWithCleanup;
        using cocos2d::ui::ScrollView::removeChild;

        using cocos2d::ui::ScrollView::addEventListenerScrollView;
        using cocos2d::ui::ScrollView::addEventListener;
    };
}

#endif
