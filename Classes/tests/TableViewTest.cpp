#include "TableViewTest.h"
#include "../wheels/cocos2dx-wheels.h"
#include "ui/CocosGUI.h"
#include <codecvt>

bool TableViewTestLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    cw::TableView *tableView = cw::TableView::create();
    tableView->setContentSize(Size(winSize.width, 200));
    tableView->setNumberOfCellsInTableViewCallback([](cw::TableView*)->ssize_t {return 20; });
    tableView->setTableCellSizeForIndexCallback([](cw::TableView *table, ssize_t idx) { return cocos2d::Size(100, 40); });
    tableView->setTableCellAtIndexCallback(std::bind(&TableViewTestLayer::_tableCellAtIndex, this, std::placeholders::_1, std::placeholders::_2));
    tableView->setDirection(ui::ScrollView::Direction::VERTICAL);

    tableView->reloadData();
    tableView->setVerticalFillOrder(cw::TableView::VerticalFillOrder::TOP_DOWN);

    tableView->setBounceEnabled(true);

    this->addChild(tableView);
    tableView->setAnchorPoint(Vec2(0.5, 0.5));
    tableView->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));

    //tableView->jumpToPercentVertical(100.0F);

    cw::ScrollBar *scrollBar = cw::createRelativeScrollBarForScrollView(tableView);

    //cw::ScrollBar *scrollBar = cw::ScrollBar::create();
    //scrollBar->setDirection(cw::ScrollBar::Direction::VERTICAL);
    //scrollBar->setViewAndContentLength(tableView->getContentSize().height, tableView->getInnerContainerSize().height);
    //scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
    this->addChild(scrollBar);
    scrollBar->setAnchorPoint(Vec2(0.5, 0.5));
    scrollBar->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));

    //tableView->addEventListener([tableView, scrollBar](Ref *sender, ui::ScrollView::EventType type) {
    //    switch (type) {
    //    case ui::ScrollView::EventType::SCROLLING:
    //    case ui::ScrollView::EventType::SCROLL_TO_TOP:
    //    case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
    //    case ui::ScrollView::EventType::BOUNCE_TOP:
    //    case ui::ScrollView::EventType::BOUNCE_BOTTOM:
    //        scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
    //        break;
    //    default:
    //        break;
    //    }
    //});

    return true;
}

cw::TableViewCell *TableViewTestLayer::_tableCellAtIndex(cw::TableView *table, ssize_t idx)
{
    cw::TableViewCell *cell = table->dequeueCell();
    if (cell == nullptr) {
        cell = cw::TableViewCell::create();

        ui::Button *button = ui::Button::create();
        button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
        button->addTouchEventListener([](Ref *sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
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
    } else {
        wchar_t wstrValue[64];
        swprintf(wstrValue, 63, L"汉字%lu", idx);
        std::string aaa;
        try {
            std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
            aaa = conv.to_bytes(wstrValue);
        }
        catch (std::exception &e) {
            CCLOG("std::exception %s", e.what());
        }

        Label *label = (Label *)cell->getChildByTag(123);
        label->setString(aaa);

        //label->setString(u8"汉字");
        //label->setString("汉字");
    }

    return cell;
}
