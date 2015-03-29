#include "RichTextExTest.h"
#include "wheels/cocos2dx-wheels.h"

#pragma execution_character_set("utf-8")

bool RichTextExTestLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    cw::RichTextEx *richText = cw::RichTextEx::create();
    richText->setEmptyLineHeight(30);
    richText->setVerticalSpace(10);
    richText->setTouchEnabled(true);

    auto elemText = cw::RichElementTextEx::create(0, Color3B::BLUE, 255, "戳这里有回调", "Arial-BoldMT", 30);
    elemText->setClickCallback([](Ref *rt, Ref *el) {
        cw::RichTextEx *richText = (cw::RichTextEx *)rt;
        switch (richText->getHorizontalAlignment()) {
        case TextHAlignment::LEFT: richText->setHorizontalAlignment(TextHAlignment::CENTER); break;
        case TextHAlignment::CENTER: richText->setHorizontalAlignment(TextHAlignment::RIGHT); break;
        case TextHAlignment::RIGHT: richText->setHorizontalAlignment(TextHAlignment::LEFT); break;
        default: break;
        }
    });
    richText->pushBackElement(elemText);

    elemText = cw::RichElementTextEx::create(0, Color3B::YELLOW, 255, "下划线", "Arial", 30);
    elemText->enableUnderline();
    richText->pushBackElement(elemText);

    elemText = cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "描边", "Arial", 30);
    elemText->enableOutline(Color3B::RED, 3);
    richText->pushBackElement(elemText);

    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "常规", "Arial-BoldMT", 30));
    richText->pushBackElement(cw::RichElementNewLineEx::create(0));
    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "换行", "Arial-BoldMT", 30));
    richText->pushBackElement(cw::RichElementNewLineEx::create(0));
    richText->pushBackElement(cw::RichElementNewLineEx::create(0));
    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "连续换行", "Arial-BoldMT", 30));
    richText->pushBackElement(cw::RichElementNewLineEx::create(0));
    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "行", "Arial-BoldMT", 30));
    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "\n\\n换行", "Arial-BoldMT", 30));
    richText->pushBackElement(cw::RichElementNewLineEx::create(0));
    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "图片乱入", "Arial-BoldMT", 30));
    richText->pushBackElement(cw::RichElementNewLineEx::create(0));
    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "LOCAL_TEXTURE", "Arial-BoldMT", 30));
    richText->pushBackElement(cw::RichElementImageEx::create(0, Color3B::WHITE, 255, "CloseNormal.png", cw::RichElementImageEx::TextureType::LOCAL));
    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "PLIST_TEXTURE", "Arial-BoldMT", 30));
    //richText->pushBackElement(cw::RichElementImageEx::create(0, Color3B::WHITE, 255, "icon_crystal.png", cw::RichElementImageEx::TextureType::PLIST));

    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "缩放", "Arial-BoldMT", 30));

    auto elemImage = cw::RichElementImageEx::create(0, Color3B::WHITE, 255, "CloseNormal.png", cw::RichElementImageEx::TextureType::LOCAL);
    elemImage->setScale(0.5);
    richText->pushBackElement(elemImage);

    richText->pushBackElement(cw::RichElementCustomNodeEx::create(0, Color3B::WHITE, 255, Sprite::create("CloseNormal.png")));

    richText->formatText();
    this->addChild(richText);

    richText->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));

    cw::addBackground(richText, Color4B(255, 0, 0, 128));

    return true;
}
