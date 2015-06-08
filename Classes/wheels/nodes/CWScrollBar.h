#ifndef _CWSCROLLBAR_H_
#define _CWSCROLLBAR_H_

#include "2d/CCLayer.h"

namespace cw {
    class ScrollBar : public cocos2d::Node {
    public:
        enum class Direction { HORIZONTAL, VERTICAL };

        ScrollBar();
        ~ScrollBar();

        static ScrollBar *create();
        virtual bool init() override;

        virtual void setColor(const cocos2d::Color3B &color) override;
        virtual void setOpacity(GLubyte opacity) override;

        void setDirection(Direction dir) { _dir = dir; }
        Direction getDirection() const { return _dir; }

        void setViewAndContentLength(float viewLength, float contentLength);
        void refreshOffset(float offset);
        void setThumbPercentage(float percentage);

        void setHideThumbIfCannotScroll(bool hide) { _hideThumbIfCannotScroll = hide; }
        bool getHideThumbIfCannotScroll() const { return _hideThumbIfCannotScroll; }

        void setBounceEnabled(bool enabled) { _bounceEnabled = enabled; }
        bool getBounceEnabled() const { return _bounceEnabled; }

        void setHideThumbWhenScrollingStopped(bool hide) { _hideThumbWhenScrollingStopped = hide; }
        bool getHideThumbWhenScrollingStopped() const { return _hideThumbWhenScrollingStopped; }

    protected:
        cocos2d::LayerColor *_thumbLayer = nullptr;
        float _contentLength = 0.0f;
        float _viewLength = 0.0f;
        Direction _dir = Direction::HORIZONTAL;
        bool _hideThumbIfCannotScroll = true;
        bool _bounceEnabled = false;
        bool _hideThumbWhenScrollingStopped = true;
    };
}

#endif
