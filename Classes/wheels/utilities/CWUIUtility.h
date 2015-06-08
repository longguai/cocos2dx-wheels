#ifndef _CW_UI_UTILITY_H_
#define _CW_UI_UTILITY_H_

namespace cocos2d {
    namespace ui {
        class ScrollView;
    }
}

namespace cw {
    class ScrollBar;
    class TableView;

    // 创建与ScrollView关联的ScrollBar
    ScrollBar *createRelativeScrollBarForScrollView(cocos2d::ui::ScrollView *scrollView);
    ScrollBar *createRelativeScrollBarForTableView(TableView *tableView);
}

#endif
