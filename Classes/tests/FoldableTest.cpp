#include "FoldableTest.h"
#include "../wheels/cocos2dx-wheels.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

bool FoldableTestLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    for (int i = 0; i < 25; ++i) {
        _foldStatus.push_back(std::make_tuple(true, Size(300, 40), Size(300, 80 + rand() % 20 * 5)));
    }

    _tableView = cw::TableView::create();
    _tableView->setContentSize(Size(winSize.width, 200));
    _tableView->setNumberOfCellsInTableViewCallback([this](cw::TableView*)->ssize_t {
        return _foldStatus.size();
    });
    _tableView->setTableCellSizeForIndexCallback([this](cw::TableView *table, ssize_t idx) {
        return std::get<0>(_foldStatus[idx]) ? std::get<1>(_foldStatus[idx]) : std::get<2>(_foldStatus[idx]);
    });
    _tableView->setTableCellAtIndexCallback(std::bind(&FoldableTestLayer::_tableCellAtIndex, this, std::placeholders::_1, std::placeholders::_2));
    _tableView->setDirection(ui::ScrollView::Direction::VERTICAL);

    _tableView->reloadData();
    _tableView->setVerticalFillOrder(cw::TableView::VerticalFillOrder::TOP_DOWN);

    _tableView->setBounceEnabled(true);

    this->addChild(_tableView);
    _tableView->setAnchorPoint(Vec2(0.5, 0.5));
    _tableView->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));

    //_tableView->jumpToPercentVertical(100.0F);

    _scrollBar = cw::createRelativeScrollBarForTableView(_tableView);

    this->addChild(_scrollBar);
    _scrollBar->setAnchorPoint(Vec2(0.5, 0.5));
    _scrollBar->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));

    return true;
}

#pragma execution_character_set("utf-8")

#define LABEL_TAG 123
#define BUTTON_TAG 456

cw::TableViewCell *FoldableTestLayer::_tableCellAtIndex(cw::TableView *table, ssize_t idx)
{
    cw::TableViewCell *cell = table->dequeueCell();
    if (cell == nullptr) {
        cell = cw::TableViewCell::create();
        cell->setIdx(idx);

        Label *label = Label::createWithSystemFont("", "Helvetica", 30.0);
        label->setAnchorPoint(Vec2(0, 1));
        label->setTag(LABEL_TAG);
        cell->addChild(label);
        label->setPositionX(0);

        ui::Button *button = ui::Button::create();
        button->loadTextureNormal("CloseNormal.png", ui::Widget::TextureResType::LOCAL);
        cell->addChild(button);
        button->setTag(BUTTON_TAG);
        button->setPositionX(300);
        button->setScale9Enabled(true);
    }

    cell->setIdx(idx);

    bool fold = std::get<0>(_foldStatus[idx]);
    const Size &cellSize = fold ? std::get<1>(_foldStatus[idx]) : std::get<2>(_foldStatus[idx]);
    char strValue[64];
    sprintf(strValue, "第%.2lu个：状态%s", idx, fold ? "收起" : "展开");

    Label *label = (Label *)cell->getChildByTag(LABEL_TAG);
    label->setString(strValue);
    label->setPositionY(cellSize.height);

    ui::Button *button = (ui::Button *)cell->getChildByTag(BUTTON_TAG);
    button->setPositionY(cellSize.height - button->getContentSize().height * 0.5);
    button->addTouchEventListener([this, table, cell, idx, label, button](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            bool &fold = std::get<0>(_foldStatus[idx]);
            fold = !fold;
            _tableView->inplaceReloadData();
        }
    });
    return cell;
}
