#include "InConvexPolygonTest.h"
#include "../wheels/cocos2dx-wheels.h"

bool InConvexPolygonTestLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    DrawNode *node = DrawNode::create();
    this->addChild(node);

    static Vec2 vertices[] = {Vec2(100, 100), Vec2(200, 80), Vec2(300, 200), Vec2(220, 300), Vec2(100, 240)};
    node->drawPolygon(vertices, sizeof(vertices) / sizeof(vertices[0]), Color4F(1.0F, 0.0F, 0.0F, 0.5F), 1.0F, Color4F(1.0F, 0.0F, 0.0F, 0.5F));

    Label *label = Label::create();
    label->setSystemFontSize(24);
    label->setString("inConvexPolygon : unknown");
    this->addChild(label);
    label->setPosition(Vec2(winSize.width * 0.5, winSize.height - 200));

    EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    std::function<void (Touch *, Event *)> func = [label](Touch *touch, Event *event) {
        if (cw::geometry::inConvexPolygon(vertices, touch->getLocation())) {
            label->setString("inConvexPolygon : true");
        } else {
            label->setString("inConvexPolygon : false");
        }
    };

    listener->onTouchBegan = [func](Touch *touch, Event *event) { func(touch, event); return true; };
    listener->onTouchMoved = func;
    listener->onTouchEnded = func;
    listener->onTouchCancelled = func;

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
