#include "NetworkTest.h"
#include "ui/CocosGUI.h"

#include "../network/CWSocketManager.h"

bool NetworkTestLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Label *label = Label::createWithTTF("Waiting for connecting", "fonts/Marker Felt.ttf", 30);
    this->addChild(label);
    label->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height - 100));

    static cw::SocketManager sm;
    sm.connectToServer("192.168.4.162", 8899, [=](cw::SocketManager *thiz, bool result) {
        label->setString(result ? "connect succeeded" : "connect failed");
        if (!result) {
            return;
        }

        ui::Button *button = ui::Button::create();
        button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
        button->setScale9Enabled(true);
        this->addChild(button);
        button->setPosition(Vec2(100, 100));
        button->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
                char vec[] = "Test1";
                sm.sendAndRegisterRecvCallback(100, vec, strlen(vec), [](char *data, int len) {
                    CCLOG("%.*s", len, data);
                });
            }
        });

        button = ui::Button::create();
        button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
        button->setScale9Enabled(true);
        this->addChild(button);
        button->setPosition(Vec2(200, 100));
        button->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
                char vec[] = "Test2";
                sm.sendAndRegisterRecvCallback(200, vec, strlen(vec), [](char *data, int len) {
                    CCLOG("%.*s", len, data);
                });

            }
        });

        button = ui::Button::create();
        button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
        button->setScale9Enabled(true);
        this->addChild(button);
        button->setPosition(Vec2(300, 100));
        button->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
                auto func3 = [](char *data, int len) {
                    CCLOG("%.*s", len, data);
                };
                
                char vec[] = "Test1";
                sm.sendAndRegisterRecvCallback(300, vec, strlen(vec), func3);
            }
        });

        button = ui::Button::create();
        button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
        button->setScale9Enabled(true);
        this->addChild(button);
        button->setPosition(Vec2(400, 100));
        button->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
                char vec[] = "Test1";
                std::function<void (char *data, size_t len)> func4 = [](char *data, int len) {
                    CCLOG("%.*s", len, data);
                };
                sm.sendAndRegisterRecvCallback(400, vec, strlen(vec), func4);
            }
        });


    });


    return true;
}
