#ifndef _UNFOLD_AND_FOLD_TEST_H_
#define _UNFOLD_AND_FOLD_TEST_H_

#include "cocos2d.h"
USING_NS_CC;

namespace cw {
    class TableViewCell;
    class TableView;
    class ScrollBar;
}

class UnfoldAndFoldTestLayer : public Layer {
public:
    virtual bool init() override;
    CREATE_FUNC(UnfoldAndFoldTestLayer)

private:
    cw::TableViewCell *_tableCellAtIndex(cw::TableView *table, ssize_t idx);
    cw::TableView *_tableView;
    cw::ScrollBar *_scrollBar;
    std::vector<std::tuple<bool, cocos2d::Size, cocos2d::Size> > _foldStatus;
};

#endif
