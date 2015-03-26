#ifndef __UITABLEVIEW__
#define __UITABLEVIEW__

#include "ui/UIScrollView.h"
#include <set>
#include <vector>

using namespace cocos2d;
using namespace cocos2d::ui;

namespace cw {
    /**
     * Abstract class for SWTableView cell node
     */
    class TableViewCell: public Widget
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
        void reset() { _idx = CC_INVALID_INDEX; }
        
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
        virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) {
            return cellSizeForTable(table);
        };
        /**
         * cell height for a given table.
         *
         * @param table table to hold the instances of Class
         * @return cell size
         */
        virtual Size cellSizeForTable(TableView *table) {
            return Size::ZERO;
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

    class TableView: public ScrollView
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

        typedef std::function<Size (TableView* table, ssize_t idx)> ccTableCellSizeForIndex;
        typedef std::function<TableViewCell* (TableView *table, ssize_t idx)> ccTableCellAtIndex;
        typedef std::function<ssize_t (TableView *table)> ccNumberOfCellsInTableView;

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
        CC_DEPRECATED_ATTRIBUTE static TableView *create(TableViewDataSource *dataSource, const Size &size);

        bool init();
        CC_DEPRECATED_ATTRIBUTE bool initWithViewSize(TableViewDataSource *dataSource, const Size &size);
        
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

        void setTableCellSizeForIndex(const ccTableCellSizeForIndex& func);
        const ccTableCellSizeForIndex& getTableCellSizeForIndex() const;
        void setTableCellAtIndex(const ccTableCellAtIndex& func);
        const ccTableCellAtIndex& getTableCellAtIndex() const;
        void setNumberOfCellsInTableView(const ccNumberOfCellsInTableView& func);
        const ccNumberOfCellsInTableView& getNumberOfCellsInTableView() const;

        /**
         * determines how cell is ordered and filled in the view.
         */
        void setVerticalFillOrder(VerticalFillOrder order);
        VerticalFillOrder getVerticalFillOrder();

        Vec2 getInnerContainerOffset() const;
        void setInnerContainerOffset(const Vec2 &offset);

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

        virtual void setDirection(ScrollView::Direction dir) override;
        
        //handle touch event
        virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
        virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
        virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
        virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;

        Vec2 maxContainerOffset();
        Vec2 minContainerOffset();

    protected:
        static Size __tableCellSizeForIndex(TableView*, ssize_t) { return Size::ZERO; }
        static TableViewCell* __tableCellAtIndex(TableView*, ssize_t) { return nullptr; }
        static ssize_t __numberOfCellsInTableView(TableView*) { return 0; }

        CC_DEPRECATED_ATTRIBUTE void _initWithViewSize(const Size &size);

        virtual void onSizeChanged() override;

        void _scrollEvent(Ref *sender, ScrollView::EventType type);

        virtual bool scrollChildren(float touchOffsetX, float touchOffsetY) override;

        long __indexFromOffset(Vec2 offset);
        long _indexFromOffset(Vec2 offset);
        Vec2 __offsetFromIndex(ssize_t index);
        Vec2 _offsetFromIndex(ssize_t index);
        
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
        Vector<TableViewCell*> _cellsUsed;
        /**
         * free list of cells
         */
        Vector<TableViewCell*> _cellsFreed;

        ccTableViewCellCallback _tableViewCellCallback;

        ccTableCellSizeForIndex _tableCellSizeForIndex;
        ccTableCellAtIndex _tableCellAtIndex;
        ccNumberOfCellsInTableView _numberOfCellsInTableView;

        Direction _oldDirection;
        
        bool _isUsedCellsDirty;
        
    public:
        void _updateContentSize();
    };
}

#endif
