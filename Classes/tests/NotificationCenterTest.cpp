#include "NotificationCenterTest.h"
#include "../wheels/CWNotificationCenter.h"

USING_NS_CC;

bool NotificationCenterTestLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    cw::NotificationCenter::getInstance()->addObserver(this, [](Ref *r) {
        CCLOG("%s %d", __FUNCTION__, __LINE__);
    }, "test1", nullptr);

    cw::NotificationCenter::getInstance()->addObserver(this, [](Ref *r) {
        CCLOG("%s %d", __FUNCTION__, __LINE__);
    }, "test2", nullptr);

    cw::NotificationCenter::getInstance()->addObserver(this, [](Ref *r) {
        CCLOG("%s %d", __FUNCTION__, __LINE__);
    }, "test2", nullptr);

    cw::NotificationCenter::getInstance()->postNotification("test1");
    cw::NotificationCenter::getInstance()->removeObserver(this, "test1");
    cw::NotificationCenter::getInstance()->postNotification("test1");
    cw::NotificationCenter::getInstance()->postNotification("test2");

    cw::NotificationCenter::getInstance()->removeAllObservers(this);
    cw::NotificationCenter::getInstance()->postNotification("test1");
    cw::NotificationCenter::getInstance()->postNotification("test2");

    return true;
}
