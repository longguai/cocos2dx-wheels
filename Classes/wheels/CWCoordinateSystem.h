#ifndef _CW_COORDINATE_SYSTEM_H_
#define _CW_COORDINATE_SYSTEM_H_

#include "2d/CCNode.h"

namespace cw {

    class CoordinateSystem : public cocos2d::Node {
    public:
        CoordinateSystem();
        virtual ~CoordinateSystem();

        static CoordinateSystem *create(int step = 100, bool showNegative = true, bool drawFullGrid = true);
        bool initWithStep(int step, bool showNegative, bool drawFullGrid);

    private:
        int _step = 100;
        bool _showNegative = true;
        bool _drawFullGrid = true;

        using cocos2d::Node::setContentSize;
        using cocos2d::Node::setAnchorPoint;
        using cocos2d::Node::setPosition;
        using cocos2d::Node::setPositionX;
        using cocos2d::Node::setPositionY;
        using cocos2d::Node::setScale;
        using cocos2d::Node::setScaleX;
        using cocos2d::Node::setScaleY;
        using cocos2d::Node::setScaleZ;

        using cocos2d::Node::addChild;
        using cocos2d::Node::removeAllChildren;
        using cocos2d::Node::removeAllChildrenWithCleanup;
        using cocos2d::Node::removeChildByTag;
        using cocos2d::Node::removeChildByName;
        using cocos2d::Node::getChildren;
        using cocos2d::Node::getChildByTag;
        using cocos2d::Node::getChildByName;
        using cocos2d::Node::getChildrenCount;
    };
}

#endif
