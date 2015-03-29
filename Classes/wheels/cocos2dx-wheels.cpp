#include "cocos2dx-wheels.h"

USING_NS_CC;

namespace cw {
    ScrollBar *createRelativeScrollBarForScrollView(ui::ScrollView *scrollView) {
        ui::ScrollView::Direction dir = scrollView->getDirection();
        ScrollBar *scrollBar = ScrollBar::create();
        if (dir == ui::ScrollView::Direction::VERTICAL) {
            scrollBar->setDirection(ScrollBar::Direction::VERTICAL);
            scrollBar->setViewAndContentLength(scrollView->getContentSize().height, scrollView->getInnerContainerSize().height);
            scrollBar->refreshOffset(scrollView->getInnerContainer()->getPositionY());
            scrollView->addEventListener([scrollView, scrollBar](Ref *sender, ui::ScrollView::EventType type) {
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
        } else if (dir == ui::ScrollView::Direction::HORIZONTAL) {
            scrollBar->setDirection(ScrollBar::Direction::HORIZONTAL);
            scrollBar->setViewAndContentLength(scrollView->getContentSize().width, scrollView->getInnerContainerSize().width);
            scrollBar->refreshOffset(scrollView->getInnerContainer()->getPositionX());
            scrollView->addEventListener([scrollView, scrollBar](Ref *sender, ui::ScrollView::EventType type) {
                switch (type) {
                    case ui::ScrollView::EventType::SCROLLING:
                    case ui::ScrollView::EventType::SCROLL_TO_LEFT:
                    case ui::ScrollView::EventType::SCROLL_TO_RIGHT:
                    case ui::ScrollView::EventType::BOUNCE_LEFT:
                    case ui::ScrollView::EventType::BOUNCE_RIGHT:
                        scrollBar->refreshOffset(scrollView->getInnerContainer()->getPositionX());
                        break;
                    default:
                        break;
                }
            });
        }
        return scrollBar;
    }

    void tableViewReloadData(TableView *tableView, ScrollBar *scrollBar) {
        switch (tableView->getDirection()) {
            case ui::ScrollView::Direction::HORIZONTAL:
                tableView->reloadData();
                scrollBar->setViewAndContentLength(tableView->getContentSize().width, tableView->getInnerContainerSize().width);
                scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionX());
                break;
            case ui::ScrollView::Direction::VERTICAL:
                tableView->reloadData();
                scrollBar->setViewAndContentLength(tableView->getContentSize().height, tableView->getInnerContainerSize().height);
                scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
                break;
            default:
                break;
        }
    }
}

