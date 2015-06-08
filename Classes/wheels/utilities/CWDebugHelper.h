#ifndef _CW_DEBUG_HELPER_H_
#define _CW_DEBUG_HELPER_H_

#include "2d/CCLayer.h"

namespace cw {

    template <class _NODE> void addBackground(_NODE *node, const cocos2d::Color4B &color) {
        using namespace cocos2d;
        const Size &size = node->getContentSize();
        LayerColor *layerColor = LayerColor::create(color, size.width, size.height);
        node->addChild(layerColor, INT_MIN);
    }

}

#endif
