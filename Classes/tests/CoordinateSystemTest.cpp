#include "CoordinateSystemTest.h"
#include "wheels/cocos2dx-wheels.h"

bool CoordinateSystemLayer::init()
{
    if (cocos2d::Layer::init())
    {
        addChild(cw::CoordinateSystem::create());
        return true;
    }
    return false;
}
