#include "CWCoordinateSystem.h"
#include "2d/CCDrawNode.h"

namespace cw {
    CoordinateSystem::CoordinateSystem()
    {

    }

    CoordinateSystem::~CoordinateSystem()
    {

    }

    CoordinateSystem *CoordinateSystem::create(int step, bool showNegative, bool drawFullGrid)
    {
        CoordinateSystem *ret = new (std::nothrow) CoordinateSystem();
        if (ret != nullptr && ret->initWithStep(step, showNegative, drawFullGrid))
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool CoordinateSystem::initWithStep(int step, bool showNegative, bool drawFullGrid)
    {
        if (!cocos2d::Node::init())
        {
            return false;
        }

        cocos2d::DrawNode *drawNode = cocos2d::DrawNode::create();
        this->addChild(drawNode);

        return true;
    }
}
