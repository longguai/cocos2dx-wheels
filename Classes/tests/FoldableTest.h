#ifndef _FOLDABLE_TEST_H_
#define _FOLDABLE_TEST_H_

#include "cocos2d.h"
USING_NS_CC;

namespace cw {
    class TableViewCell;
    class TableView;
    class ScrollBar;
}

class FoldableTestLayer : public Layer {
public:
    virtual bool init() override;
    CREATE_FUNC(FoldableTestLayer)

private:
    cw::TableViewCell *_tableCellAtIndex(cw::TableView *table, ssize_t idx);
    cw::TableView *_tableView;
    cw::ScrollBar *_scrollBar;
    std::vector<std::tuple<bool, cocos2d::Size, cocos2d::Size> > _foldStatus;
};

#endif
