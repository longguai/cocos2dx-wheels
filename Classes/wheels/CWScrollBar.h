#ifndef _CWSCROLLBAR_H_
#define _CWSCROLLBAR_H_

//#include "2d/CCLayer.h"
#include "cocos2d.h"

namespace cw {
    class ScrollBar : public cocos2d::__NodeRGBA {
    public:
        enum class Direction { HORIZONTAL, VERTICAL };

        ScrollBar();
        ~ScrollBar();

        static ScrollBar *create();
        virtual bool init() override;

        void setDirection(Direction dir) { _dir = dir; }
        Direction getDirection() const { return _dir; }

        void setViewAndContentLength(float viewLength, float contentLength);
        void refreshOffset(float offset);
        void setThumbPercentage(float percentage);

    protected:
        cocos2d::LayerColor *_thumbLayer = nullptr;
        float _contentLength = 0.0f;
        float _viewLength = 0.0f;
        Direction _dir = Direction::HORIZONTAL;
        bool _hideThumbIfCannotScroll = true;
        bool _bounceEnabled = false;//
        bool _hideThumbWhenScrollingStopped = true;
    };
}

#endif
