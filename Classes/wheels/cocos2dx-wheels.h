#ifndef _COCOS2DX_WHEELS_H_
#define _COCOS2DX_WHEELS_H_

#include "CWCallFuncDelegate.h"
#include "CWCoordinateSystem.h"
#include "CWDebugHelper.h"
#include "CWGeometry.h"
#include "CWLayoutFunctions.h"
#include "CWMacros.h"
#include "CWNotificationCenter.h"
#include "CWRefConverter.h"
#include "CWRefWrap.h"
#include "CWRichTextEx.h"
#include "CWScrollBar.h"
#include "CWTableView.h"
#include "CWVariableArgumentList.h"

namespace cw {
    // 创建与ScrollView关联的ScrollBar
    ScrollBar *createRelativeScrollBarForScrollView(cocos2d::ui::ScrollView *scrollView);
    ScrollBar *createRelativeScrollBarForTableView(TableView *tableView);
}

#endif
