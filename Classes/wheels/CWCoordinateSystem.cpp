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

        _step = step;
        _showNegative = showNegative;
        _drawFullGrid = drawFullGrid;

        cocos2d::DrawNode *drawNode = cocos2d::DrawNode::create();
        this->addChild(drawNode);

        const cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

        int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
        if (_showNegative) {
            while (x0 > -winSize.width) x0 -= _step;
            while (y0 > -winSize.height) y0 -= _step;
        }
        while (x1 < winSize.width) x1 += _step;
        while (y1 < winSize.height) y1 += _step;

        drawNode->drawLine(cocos2d::Vec2(0.0f, y0), cocos2d::Vec2(0.0f, y1), cocos2d::Color4F::WHITE);
        drawNode->drawLine(cocos2d::Vec2(x0, 0.0f), cocos2d::Vec2(x1, 0.0f), cocos2d::Color4F::WHITE);

        char str[64];
        cocos2d::Label *label;

        for (int x = x0; x < x1; x += _step) {
            if (x == 0) continue;
            label = cocos2d::Label::create();
            label->setSystemFontSize(20);
            snprintf(str, 63, "%d", x);
            label->setString(str);
            this->addChild(label);
            label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
            label->setPosition(cocos2d::Vec2(x, 10));

            if (_drawFullGrid) {
                drawNode->drawLine(cocos2d::Vec2(x, y0), cocos2d::Vec2(x, y1), cocos2d::Color4F::WHITE);
            } else {
                drawNode->drawLine(cocos2d::Vec2(x, 0.0f), cocos2d::Vec2(x, 5.0f), cocos2d::Color4F::WHITE);
            }
        }

        for (int y = y0; y < y1; y += _step) {
            if (y == 0) continue;
            label = cocos2d::Label::create();
            label->setSystemFontSize(20);
            snprintf(str, 63, "%d", y);
            label->setString(str);
            this->addChild(label);
            label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
            label->setPosition(cocos2d::Vec2(10, y));

            if (_drawFullGrid) {
                drawNode->drawLine(cocos2d::Vec2(x0, y), cocos2d::Vec2(x1, y), cocos2d::Color4F::WHITE);
            } else {
                drawNode->drawLine(cocos2d::Vec2(0.0f, y), cocos2d::Vec2(5.0f, y), cocos2d::Color4F::WHITE);
            }
        }

        return true;
    }
}
