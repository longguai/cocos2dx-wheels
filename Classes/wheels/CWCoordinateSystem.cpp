#include "CWCoordinateSystem.h"
#include "2d/CCDrawNode.h"
#include "base/CCDirector.h"
#include "2d/CCLabel.h"

namespace cw {

    CoordinateSystem::CoordinateSystem() {

    }

    CoordinateSystem::~CoordinateSystem() {

    }

    CoordinateSystem *CoordinateSystem::create(int step, bool showNegative, bool drawFullGrid) {
        CoordinateSystem *ret = new (std::nothrow) CoordinateSystem();
        if (ret != nullptr && ret->initWithStep(step, showNegative, drawFullGrid)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool CoordinateSystem::initWithStep(int step, bool showNegative, bool drawFullGrid) {
        if (!cocos2d::Node::init()) {
            return false;
        }

        cocos2d::DrawNode *drawNode = cocos2d::DrawNode::create();
        this->addChild(drawNode);

        const cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

        int x = 0, y = 0;
        if (_showNegative) {
            while (x > -winSize.width) x-= step;
            while (y > -winSize.height) y -= step;
        }
        const int xx = x, yy = y;
        drawNode->drawLine(cocos2d::Vec2(0.0f, yy), cocos2d::Vec2(0.0f, winSize.height), cocos2d::Color4F::WHITE);
        drawNode->drawLine(cocos2d::Vec2(xx, 0.0f), cocos2d::Vec2(winSize.width, 0.0f), cocos2d::Color4F::WHITE);

        char str[64];
        while (x < winSize.width) {
            cocos2d::Label *label = cocos2d::Label::create();
            label->setSystemFontSize(20);
            snprintf(str, 63, "%d", x);
            label->setString(str);
            this->addChild(label);
            label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
            label->setPosition(cocos2d::Vec2(x, 5));

            if (_drawFullGrid) {
                drawNode->drawLine(cocos2d::Vec2(x, yy), cocos2d::Vec2(x, winSize.height), cocos2d::Color4F::WHITE);
            } else {
                drawNode->drawLine(cocos2d::Vec2(x, 0.0f), cocos2d::Vec2(x, 5.0f), cocos2d::Color4F::WHITE);
            }
            x += step;
        }

        while (y < winSize.height) {
            cocos2d::Label *label = cocos2d::Label::create();
            label->setSystemFontSize(20);
            snprintf(str, 63, "%d", y);
            label->setString(str);
            this->addChild(label);
            label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
            label->setPosition(cocos2d::Vec2(5, y));

            if (_drawFullGrid) {
                drawNode->drawLine(cocos2d::Vec2(xx, y), cocos2d::Vec2(winSize.width, y), cocos2d::Color4F::WHITE);
            } else {
                drawNode->drawLine(cocos2d::Vec2(0.0f, y), cocos2d::Vec2(5.0f, y), cocos2d::Color4F::WHITE);
            }
            y += step;
        }

        return true;
    }
}
