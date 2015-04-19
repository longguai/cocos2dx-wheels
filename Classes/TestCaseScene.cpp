#include "TestCaseScene.h"
#include <functional>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "tests/ArrangeTest.h"
#include "tests/InConvexPolygonTest.h"
#include "tests/RichTextExTest.h"
#include "tests/TableViewTest.h"
#include "tests/FoldableTest.h"

USING_NS_CC;

static struct {
    std::string title;
    std::function<Layer *()> createFunc;
} sTestCases[] = {
    { "RichTextEx Test", &RichTextExTestLayer::create },
    { "TableView Test", &TableViewTestLayer::create },
    { "Arrange Test", &ArrangeTestLayer::create },
    { "InConvexPolygon Test", &InConvexPolygonTestLayer::create },
    { "Foldable Test", &FoldableTestLayer::create }
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
