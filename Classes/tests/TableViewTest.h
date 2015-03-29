#ifndef _TABLE_VIEW_TEST_H_
#define _TABLE_VIEW_TEST_H_

#include "cocos2d.h"
USING_NS_CC;

namespace cw {
    class TableViewCell;
    class TableView;
}

class TableViewTestLayer : public Layer {
public:
    virtual bool init() override;
    CREATE_FUNC(TableViewTestLayer)

private:
    cw::TableViewCell *_tableCellAtIndex(cw::TableView *table, ssize_t idx);
};

#endif
