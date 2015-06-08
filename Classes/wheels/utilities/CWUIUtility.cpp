#include "CWUIUtility.h"

#include "../widgets/CWTableView.h"
#include "../nodes/CWScrollBar.h"

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

    ScrollBar *createRelativeScrollBarForTableView(TableView *tableView) {
        ui::ScrollView::Direction dir = tableView->getDirection();
        ScrollBar *scrollBar = ScrollBar::create();
        if (dir == ui::ScrollView::Direction::VERTICAL) {
            scrollBar->setDirection(ScrollBar::Direction::VERTICAL);
            scrollBar->setViewAndContentLength(tableView->getContentSize().height, tableView->getInnerContainerSize().height);
            scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
            tableView->setScrollBarEventCallback([scrollBar](TableView *tableView, TableView::ScrollBarEvent event) {
                switch (event) {
                case TableView::ScrollBarEvent::REFRESH_LENGTH:
                    scrollBar->setViewAndContentLength(tableView->getContentSize().height, tableView->getInnerContainerSize().height);
                    break;
                case TableView::ScrollBarEvent::REFRESH_OFFSET:
                    scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionY());
                    break;
                default:
                    break;
                }
            });
        } else if (dir == ui::ScrollView::Direction::HORIZONTAL) {
            scrollBar->setDirection(ScrollBar::Direction::HORIZONTAL);
            scrollBar->setViewAndContentLength(tableView->getContentSize().width, tableView->getInnerContainerSize().width);
            scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionX());
            tableView->setScrollBarEventCallback([scrollBar](TableView *tableView, TableView::ScrollBarEvent event) {
                switch (event) {
                case TableView::ScrollBarEvent::REFRESH_LENGTH:
                    scrollBar->setViewAndContentLength(tableView->getContentSize().width, tableView->getInnerContainerSize().width);
                    break;
                case TableView::ScrollBarEvent::REFRESH_OFFSET:
                    scrollBar->refreshOffset(tableView->getInnerContainer()->getPositionX());
                    break;
                default:
                    break;
                }
            });
        }
        return scrollBar;
    }
}
