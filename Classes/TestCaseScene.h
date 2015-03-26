#ifndef __cocos2dx_wheels__TestCaseScene__
#define __cocos2dx_wheels__TestCaseScene__

#include "cocos2d.h"
#include "wheels/cocos2dx-wheels.h"

class TestCaseScene: public cocos2d::Scene {
public:
    CREATE_FUNC(TestCaseScene);
    bool init();
private:
    cocos2d::Node *_innerNode;
};

#endif
