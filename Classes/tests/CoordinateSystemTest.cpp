#include "CoordinateSystemTest.h"
#include "../wheels/CWCoordinateSystem.h"

bool CoordinateSystemLayer::init()
{
    if (cocos2d::Layer::init())
    {
        addChild(cw::CoordinateSystem::create());
        return true;
    }
    return false;
}
