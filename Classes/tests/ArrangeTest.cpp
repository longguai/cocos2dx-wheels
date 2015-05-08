#include "ArrangeTest.h"
#include "../wheels/cocos2dx-wheels.h"

USING_NS_CC;

bool ArrangeTestLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();
    std::vector<Node *> arr;
    for (int i = 0; i < 20; ++i) {
        Sprite *s = Sprite::create("CloseNormal.png");
        arr.push_back(s);
    }
    Node *node = cw::arrangeAsMatrixByColumn<Node>(arr, 5, Size(10, 10));
    this->addChild(node);
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));

    cw::addBackground(node, Color4B(255, 0, 0, 128));

    return true;
}
