#include "RippleTest.h"
#include "../wheels/CWRippleSprite.h"

USING_NS_CC;

bool RippleTestLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();
    Vec2 midPos = Vec2(winSize.width * 0.5, winSize.height * 0.5);

    auto rs = cw::RippleSprite::create("HelloWorld.png");
    this->addChild(rs);
    rs->setPosition(midPos);
    rs->runAction(RepeatForever::create(Sequence::create(CallFunc::create([rs, midPos]() {
        rs->touch(midPos, 512, 12);
    }), DelayTime::create(2), nullptr)));
    rs->scheduleUpdate();
    
    return true;
}
