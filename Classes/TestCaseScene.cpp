#include "TestCaseScene.h"
#include <functional>
#include <codecvt>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

static Layer *testRichTextEx();
static Layer *testTableView();
static Layer *testArrange();

static struct {
    std::string title;
    std::function<Layer *()> createFunc;
} sTestCases[] = {
    {"RichTextEx Test", testRichTextEx},
    {"TableView Test", testTableView},
    {"Arrange Test", testArrange}
};

bool TestCaseScene::init() {
    if (!Scene::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    Node *innerNode = Node::create();
    float height = 0.0F;
    size_t len = sizeof(sTestCases) / sizeof(*sTestCases);
    for (size_t j = len; j > 0; --j) {
        ui::Button *button = ui::Button::create();
        button->setTitleText(sTestCases[j - 1].title);
        button->setTitleFontSize(32);
        button->setScale9Enabled(true);

        innerNode->addChild(button);
        button->setPosition(Vec2(winSize.width * 0.5, 50 * (len - j) + 30));
        height += 50;

        button->addTouchEventListener([this, innerNode, j](Ref *, ui::Widget::TouchEventType type) {
            if (type != ui::Widget::TouchEventType::ENDED) {
                return;
            }
            Size winSize = Director::getInstance()->getWinSize();

            _innerNode->removeFromParentAndCleanup(true);
            Layer *layer = sTestCases[j - 1].createFunc();
            this->addChild(layer);

            Label *titleLabel = Label::create();
            titleLabel->setString(sTestCases[j - 1].title);
            titleLabel->setSystemFontSize(30);
            titleLabel->setPosition(Vec2(winSize.width * 0.5, winSize.height - 20));
            layer->addChild(titleLabel);

            ui::Button *button = ui::Button::create();
            button->setTitleText("reset");
            button->setTitleFontSize(32);
            button->setScale9Enabled(true);
            button->addTouchEventListener([](Ref *, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    CCLOG("ENDED");
                    Director::getInstance()->replaceScene(TestCaseScene::create());
                }
            });


            this->addChild(button, INT_MAX);
            button->setAnchorPoint(Vec2(1.0F, 1.0F));
            button->ignoreAnchorPointForPosition(false);
            button->setPosition(Vec2(winSize.width, winSize.height));
        });
    }
    innerNode->setContentSize(Size(winSize.width, height));
    if (height < winSize.height) {
        innerNode->setAnchorPoint(Vec2(0.5, 0.5));
        innerNode->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
        this->addChild(innerNode);
        _innerNode = innerNode;
    } else {
        Node *rootNode = Node::create();
        rootNode->setContentSize(winSize);
        this->addChild(rootNode);
        _innerNode = rootNode;

        ui::ScrollView *scrollView = ui::ScrollView::create();
        scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        scrollView->setBounceEnabled(true);
        scrollView->setContentSize(winSize);
        scrollView->setInnerContainerSize(Size(winSize.width, height));

        scrollView->addChild(innerNode);
        rootNode->addChild(scrollView);
        scrollView->setAnchorPoint(Vec2(0.5, 0.5));
        scrollView->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));

        cw::ScrollBar *scrollBar = cw::ScrollBar::create();
        scrollBar->setDirection(cw::ScrollBar::Direction::VERTICAL);
        scrollBar->setHideThumbWhenScrollingStopped(false);
        scrollBar->setViewAndContentLength(scrollView->getContentSize().height, scrollView->getInnerContainerSize().height);
        scrollBar->setAnchorPoint(Vec2(0.5, 0.5));
        scrollBar->setPosition(Vec2(winSize.width - 10, winSize.height * 0.5));
        rootNode->addChild(scrollBar);
        scrollBar->refreshOffset(scrollView->getInnerContainer()->getPositionY());
        scrollBar->setColor(Color3B(255, 255, 255));
        
        scrollView->addEventListener([scrollBar, scrollView](Ref *sender, ui::ScrollView::EventType type) {
            switch (type) {
                case ui::ScrollView::EventType::SCROLLING:
                case ui::ScrollView::EventType::SCROLL_TO_TOP:
                case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
                case ui::ScrollView::EventType::BOUNCE_TOP:
                case ui::ScrollView::EventType::BOUNCE_BOTTOM:
                    scrollBar->refreshOffset(scrollView->getInnerContainer()->getPositionY());
                    break;
                default:
                    break;
            }
        });
    }
    return true;
}

Layer *testRichTextEx() {
    Size winSize = Director::getInstance()->getWinSize();
    Layer *layer = Layer::create();

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
    layer->addChild(richText);

    richText->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    
    cw::addBackground(richText, Color4B(255, 0, 0, 128));

    return layer;
}

cw::TableViewCell *_tableCellAtIndex(cw::TableView *table, ssize_t idx)
{
    using namespace cocos2d;
    
    cw::TableViewCell *cell = table->dequeueCell();
    if (cell == nullptr) {
        cell = cw::TableViewCell::create();
        
        ui::Button *button = ui::Button::create();
        button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
        button->addTouchEventListener([](Ref *sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                CCLOG("uibutton clicked");
            }
        });
        cell->addChild(button);
        button->setPosition(Vec2(200, button->getContentSize().height * 0.5));
        button->setScale9Enabled(true);
        
        char strValue[64];
        sprintf(strValue, "%lu", idx);
        
        Label *label = Label::createWithSystemFont(strValue, "Helvetica", 30.0);
        label->setAnchorPoint(Vec2(0, 0));
        label->setTag(123);
        cell->addChild(label);
    }
    else {
        wchar_t wstrValue[64];
        swprintf(wstrValue, 63, L"汉字%lu", idx);
        std::string aaa;
        try {
            std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
            aaa = conv.to_bytes(wstrValue);
        } catch (std::exception &e) {
            CCLOG("std::exception %s", e.what());
        }
        
        Label *label = (Label *)cell->getChildByTag(123);
        label->setString(aaa);
        
        //label->setString(u8"汉字");
        //label->setString("汉字");
    }
    
    return cell;
}

Layer *testTableView() {
    Size winSize = Director::getInstance()->getWinSize();
    Layer *layer = Layer::create();

    cw::TableView *tableView = cw::TableView::create();
    tableView->setContentSize(Size(winSize.width, 200));
    tableView->setNumberOfCellsInTableView([](cw::TableView*)->ssize_t {return 20; });
    tableView->setTableCellSizeForIndex([](cw::TableView *table, ssize_t idx) { return cocos2d::Size(100, 40); });
    tableView->setTableCellAtIndex(&_tableCellAtIndex);
    tableView->setDirection(ui::ScrollView::Direction::HORIZONTAL);

    tableView->reloadData();
    tableView->setVerticalFillOrder(cw::TableView::VerticalFillOrder::TOP_DOWN);

    tableView->setBounceEnabled(true);
    
    layer->addChild(tableView);
    tableView->setAnchorPoint(Vec2(0.5, 0.5));
    tableView->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    
    //tableView->jumpToPercentVertical(100.0F);
    
    cw::ScrollBar *scrollBar = cw::ScrollBar::create();
    scrollBar->setDirection(cw::ScrollBar::Direction::VERTICAL);
    scrollBar->setViewAndContentLength(tableView->getContentSize().height, tableView->getInnerContainerSize().height);
    scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
    layer->addChild(scrollBar);
    scrollBar->setAnchorPoint(Vec2(0.5, 0.5));
    scrollBar->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    
    tableView->addEventListener([tableView, scrollBar](Ref *sender, ui::ScrollView::EventType type) {
        switch (type) {
            case ui::ScrollView::EventType::SCROLLING:
            case ui::ScrollView::EventType::SCROLL_TO_TOP:
            case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            case ui::ScrollView::EventType::BOUNCE_TOP:
            case ui::ScrollView::EventType::BOUNCE_BOTTOM:
                scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
                break;
            default:
                break;
        }
    });

    return layer;
}

Layer *testArrange() {
    using namespace cocos2d;
    Size winSize = Director::getInstance()->getWinSize();
    Layer *layer = Layer::create();
    std::vector<Node *> arr;
    for (int i = 0; i < 20; ++i) {
        Sprite *s = Sprite::create("CloseNormal.png");
        arr.push_back(s);
    }
    Node *node = cw::arrangeAsMatrixByColumn<Node>(arr, 5, Size(10, 10));
    layer->addChild(node);

    cw::addBackground(node, Color4B(255, 0, 0, 128));

    return layer;
}