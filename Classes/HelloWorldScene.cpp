#include "HelloWorldScene.h"
#include "ui/UIButton.h"

#include <codecvt>

#include "wheels/CWScrollBar.h"
#include "wheels/CWTableView.h"
#include "wheels/CWRichTextEx.h"

USING_NS_CC;

template <class _NODE> void addBackground(_NODE *node, const Color4B &color4b) {
    //Node *n = dynamic_cast<Node *>(node);
    auto n = node;
    if (n != nullptr) {
        const Size &size = n->getContentSize();
        LayerColor *layerColor = LayerColor::create(color4b, size.width, size.height);
        n->addChild(layerColor, INT_MIN);
    }
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

//#pragma execution_character_set("utf-8") 

    static cw::TableView *tableView = nullptr;

    ui::Button *button = ui::Button::create();
    button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
    button->addTouchEventListener([this, visibleSize](Ref *sender, ui::Widget::TouchEventType type) {
        if (type != ui::Widget::TouchEventType::ENDED)
        {
            return;
        }
        CCLOG("fda uibutton clicked");
        if (tableView != nullptr)
        {
            return;
            //tableView->removeFromParentAndCleanup(true);
        }

        tableView = cw::TableView::create();
        tableView->setContentSize(Size(visibleSize.width, 200));
        tableView->setNumberOfCellsInTableView([](cw::TableView*)->ssize_t {return 20; });
        tableView->setTableCellSizeForIndex([](cw::TableView *table, ssize_t idx) { return cocos2d::Size(100, 40); });
        tableView->setTableCellAtIndex(std::bind(&HelloWorld::tableCellAtIndex, this, std::placeholders::_1, std::placeholders::_2));

        //tableView->setDirection(ui::TableView::Direction::HORIZONTAL);

        tableView->reloadData();
        tableView->setVerticalFillOrder(cw::TableView::VerticalFillOrder::TOP_DOWN);


        tableView->setBounceEnabled(true);

        this->addChild(tableView);
        tableView->setAnchorPoint(Vec2(0.5, 0.5));
        tableView->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));

        //tableView->jumpToPercentVertical(100.0F);

        cw::ScrollBar *scrollBar = cw::ScrollBar::create();
        scrollBar->setDirection(cw::ScrollBar::Direction::VERTICAL);
        scrollBar->setViewAndContentLength(tableView->getContentSize().height, tableView->getInnerContainerSize().height);
        scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
        this->addChild(scrollBar);
        scrollBar->setAnchorPoint(Vec2(0.5, 0.5));
        scrollBar->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));

        tableView->addEventListener([scrollBar](Ref *sender, ui::ScrollView::EventType type) {
            switch (type) {
            case ui::ScrollView::EventType::SCROLLING:
            case ui::ScrollView::EventType::SCROLL_TO_TOP:
            case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
                scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
                break;
            default:
                break;
            }
        });

        addBackground(tableView, Color4B(0, 255, 0, 128));
        addBackground(scrollBar, Color4B(0, 0, 255, 128));
    });
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setPosition(Vec2(50, 50));

    return true;


    cw::RichTextEx *richText = cw::RichTextEx::create();
    richText->ignoreContentAdaptWithSize(false);
    richText->setContentSize(Size(200, 0));
    richText->pushBackElement(cw::RichElementTextEx::create(0, Color3B::WHITE, 255, "rpqn", "Helvetica", 30));
    richText->pushBackElement(cw::RichElementImageEx::create(0, Color3B::WHITE, 255, "CloseNormal.png", cw::RichElementImageEx::TextureType::LOCAL));
    richText->setTouchEnabled(true);
    this->addChild(richText);
    richText->formatText();
    richText->setPosition(Vec2(150, 50));

    LayerColor* layerColor = LayerColor::create(Color4B(255, 0, 0, 127));
    layerColor->setContentSize(richText->getContentSize());
    richText->addChild(layerColor, -100);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

cw::TableViewCell *HelloWorld::tableCellAtIndex(cw::TableView *table, ssize_t idx)
{
    using namespace cocos2d;

    cw::TableViewCell *cell = table->dequeueCell();
    if (cell == nullptr) {
        cell = cw::TableViewCell::create();

        ui::Button *button = ui::Button::create();
        button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
        //button->addTouchEventListener([](Ref *sender, ui::Widget::TouchEventType type) {
        //    if (type != ui::Widget::TouchEventType::ENDED)
        //    {
        //        return;
        //    }
        //    CCLOG("uibutton clicked");
        //});
        cell->addChild(button);
        button->setPosition(Vec2(200, button->getContentSize().height * 0.5));
        button->setScale9Enabled(true);

        //var sprite = new cc.Sprite("CloseNormal.png");
        //sprite.anchorX = 0;
        //sprite.anchorY = 0;
        //sprite.x = 0;
        //sprite.y = 0;
        //cell.addChild(sprite);

        //cell.addChild(new cc.LayerColor(cc.color(Math.random() * 255, Math.random() * 255, Math.random() * 255, Math.random() * 255)));

        char strValue[64];
        sprintf(strValue, "%lu", idx);

        LabelTTF *label = LabelTTF::create(strValue, "Helvetica", 30.0);
        label->setAnchorPoint(Vec2(0, 0));
        label->setTag(123);
        cell->addChild(label);

        //ui::RichTextEx *richText = ui::RichTextEx::create();
        //richText->ignoreContentAdaptWithSize(false);
        //richText->setContentSize(Size(200, 0));
        //richText->pushBackElement(ui::RichElementTextEx::create(0, Color3B::WHITE, 255, "rpqn", "Helvetica", 30));
        //richText->pushBackElement(ui::RichElementImageEx::create(0, Color3B::WHITE, 255, "CloseNormal.png"));
        //richText->setTouchEnabled(true);
        //cell->addChild(richText);
        //richText->setPosition(Vec2(50, 20));

        CCLOG("new");
    }
    else {
        wchar_t wstrValue[64];
        swprintf(wstrValue, L"ºº×Ö%lu", idx);

        std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;

        LabelTTF *label = (LabelTTF *)cell->getChildByTag(123);
        label->setString(conv.to_bytes(wstrValue));
        //label->setString(u8"ºº×Ö");
        //label->setString("ºº×Ö");
        CCLOG("reuse + ");
    }

    return cell;
}
