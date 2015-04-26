#ifndef _CoordinateSystemLayer_H_
#define _CoordinateSystemLayer_H_

#include "cocos2d.h"

class CoordinateSystemLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(CoordinateSystemLayer);
    virtual bool init() override;
};

#endif
