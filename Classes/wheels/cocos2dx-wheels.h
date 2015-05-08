#ifndef _COCOS2DX_WHEELS_H_
#define _COCOS2DX_WHEELS_H_

#include "CWRichTextEx.h"
#include "CWScrollBar.h"
#include "CWTableView.h"
#include "CWGeometry.h"
#include "CWLayoutFunctions.h"
#include "CWDebugHelper.h"
#include "CWVariableArgumentList.h"

#include "base/CCVector.h"
#include "2d/CCActionInterval.h"

namespace cw {
    // 创建与ScrollView关联的ScrollBar
    ScrollBar *createRelativeScrollBarForScrollView(cocos2d::ui::ScrollView *scrollView);
    void tableViewReloadData(TableView *tableView, ScrollBar *scrollBar);
    void tableViewInplaceReloadData(TableView *tableView, ScrollBar *scrollBar);
}

#endif
