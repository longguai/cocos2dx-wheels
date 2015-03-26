#include "CWScrollBar.h"

USING_NS_CC;

namespace cw {
    ScrollBar::ScrollBar() {

    }

    ScrollBar::~ScrollBar() {

    }

    ScrollBar *ScrollBar::create() {
        ScrollBar *ret = new (std::nothrow) ScrollBar();
        if (ret != nullptr && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool ScrollBar::init() {
        if (!Node::init()) {
            return false;
        }

        this->setContentSize(Size(10.0F, 10.0F));

        _thumbLayer = LayerColor::create(Color4B(255, 0, 0, 128), 10.0F, 10.0F);
        _thumbLayer->ignoreAnchorPointForPosition(false);
        _thumbLayer->setAnchorPoint(Vec2(0.5F, 0.5F));
        _thumbLayer->setPosition(Vec2(5.0F, 5.0F));

        this->addChild(_thumbLayer);
        return true;
    }

    void ScrollBar::setViewAndContentLength(float viewLength, float contentLength) {
        if (_viewLength == viewLength && _contentLength == contentLength) {
            return;
        }

        _viewLength = viewLength;
        _contentLength = contentLength;

        if (_dir == Direction::VERTICAL) {
            setContentSize(Size(_contentSize.width, viewLength));
        }
        else {
            setContentSize(Size(viewLength, _contentSize.height));
        }

        if (viewLength < contentLength) {
            _thumbLayer->setVisible(true);

            const Size &thumbSize = _thumbLayer->getContentSize();

            if (_dir == Direction::VERTICAL) {
                float scaleY = viewLength / contentLength * thumbSize.height;
                _thumbLayer->setScaleY(scaleY);
            }
            else {
                float scaleX = viewLength / contentLength * thumbSize.width;
                _thumbLayer->setScaleX(scaleX);
            }
        }
        else {
            if (_hideThumbIfCannotScroll) {
                _thumbLayer->setVisible(false);
            }
        }

        if (_hideThumbWhenScrollingStopped) {
            _thumbLayer->stopAllActions();
            _thumbLayer->setOpacity(0);
        }
    }

    void ScrollBar::refreshOffset(float offset) {
        if (_contentLength < _viewLength) {
            if (_hideThumbIfCannotScroll) {
                if (_thumbLayer->isVisible()) {
                    _thumbLayer->setVisible(false);
                }
            }
        }
        else {
            float ratioOffset = (-offset) + _viewLength * 0.5;
            const Size &contentSize = _contentSize;
            float contentSizeLength, thumbSizeLength;
            void (Node::*setScaleFunc)(float);
            void (Node::*setPositionFunc)(float);

            _thumbLayer->setPosition(Vec2(contentSize.width * 0.5, contentSize.height * 0.5));

            if (_dir == Direction::VERTICAL) {
                contentSizeLength = contentSize.height;
                thumbSizeLength = _thumbLayer->getContentSize().height;
                setScaleFunc = &Node::setScaleY;
                setPositionFunc = &Node::setPositionY;
            }
            else {
                contentSizeLength = contentSize.width;
                thumbSizeLength = _thumbLayer->getContentSize().width;
                setScaleFunc = &Node::setScaleX;
                setPositionFunc = &Node::setPositionX;
            }

            ratioOffset = ratioOffset * contentSizeLength / _contentLength;

            bool outOfMinBoundary = false;
            bool outOfMaxBoundary = false;
            float orginRatio = _viewLength * contentSizeLength / _contentLength;
            float ratioView = orginRatio;
            if (ratioOffset < ratioView * 0.5) {
                ratioView = ratioOffset * 2;
                outOfMinBoundary = true;
            }
            if (ratioOffset > contentSizeLength - ratioView * 0.5) {
                ratioView = (contentSizeLength - ratioOffset) * 2;
                outOfMaxBoundary = true;
            }

            if (!outOfMinBoundary && !outOfMaxBoundary) {
                (_thumbLayer->*setScaleFunc)(orginRatio / thumbSizeLength);
                (_thumbLayer->*setPositionFunc)(ratioOffset);
            }
            else {
                if (_bounceEnabled) {
                    (_thumbLayer->*setScaleFunc)(ratioView / thumbSizeLength);
                    (_thumbLayer->*setPositionFunc)(ratioOffset);
                }
                else {
                    (_thumbLayer->*setScaleFunc)(orginRatio / thumbSizeLength);
                    if (outOfMinBoundary) {
                        (_thumbLayer->*setPositionFunc)(orginRatio * 0.5);
                    }
                    else {
                        (_thumbLayer->*setPositionFunc)(contentSizeLength - orginRatio * 0.5);
                    }
                }
            }

            if (_hideThumbWhenScrollingStopped) {
                _thumbLayer->stopAllActions();
                _thumbLayer->setOpacity(127);
                _thumbLayer->runAction(Sequence::create(DelayTime::create(0.5F), FadeOut::create(0.5F), nullptr));
            }

            if (!_thumbLayer->isVisible()) {
                _thumbLayer->setVisible(true);
            }
        }
    }

    void ScrollBar::setThumbPercentage(float percentage) {
        if (_viewLength < _contentLength) {
            float offset = (_viewLength - _contentLength) * percentage / 100;
            refreshOffset(offset);
        }
    }
}
