#ifndef _COCOS2DX_WHEELS_H_
#define _COCOS2DX_WHEELS_H_

#include "CWRichTextEx.h"
#include "CWScrollBar.h"
#include "CWTableView.h"
#include "CWGeometry.h"
#include "base/CCVector.h"
#include "2d/CCActionInterval.h"

namespace cw {
    ScrollBar *createRelativeScrollBarForScrollView(cocos2d::ui::ScrollView *scrollView);
    void tableViewReloadData(cw::TableView *tableView, cw::ScrollBar *scrollBar);

    template <class _NODE> void addBackground(_NODE *node, const cocos2d::Color4B &color) {
        using namespace cocos2d;
        const Size &size = node->getContentSize();
        LayerColor *layerColor = LayerColor::create(color, size.width, size.height);
        node->addChild(layerColor, INT_MIN);
    }

    template <class _NODE, class _VECTOR>
    _NODE *arrangeAsMatrixByColumn(const _VECTOR &array, int col, const cocos2d::Size &gapSize, cocos2d::Size eachContentSize = cocos2d::Size(0.0f, 0.0f)) {
        using namespace cocos2d;

        _NODE *root = _NODE::create();
        int cnt = array.size();
        if (cnt == 0) {
            return root;
        }

        int fullRow = cnt / col;
        int rem = cnt % col;
        int row = fullRow + (rem > 0 ? 1 : 0);

        if (eachContentSize.equals(Size(0.0F, 0.0F))) {
            eachContentSize = array.at(0)->getContentSize();
        }

        Size size(0.0F, 0.0F);
        size.width = ((col == 1) ? (eachContentSize.width) : (eachContentSize.width * col + gapSize.width * (col - 1)));
        size.height = ((row == 1) ? (eachContentSize.height) : (eachContentSize.height * row + gapSize.height * (row - 1)));

        root->setContentSize(size);

        float posX = eachContentSize.width * 0.5;
        float posY = size.height - eachContentSize.height * 0.5;
        for (int i = 0; i < cnt; ++i) {
            Node *child = array.at(i);
            root->addChild(child);
            child->setAnchorPoint(Vec2(0.5F, 0.5F));
            child->setPosition(Vec2(posX, posY));

            if (i % col < col - 1) {
                posX += (eachContentSize.width + gapSize.width);
            } else {
                posX = eachContentSize.width * 0.5;
                posY -= (eachContentSize.height + gapSize.height);
            }
        }

        return root;
    }

    template <class _T0, class ..._ARGS> struct VALPacker {
        template <class _T>
        static void pack(cocos2d::Vector<_T> &vec, _T0 t0, _ARGS...args) {
            vec.pushBack(t0);
            VALPacker<_ARGS...>::pack(vec, args...);
        }
    };

    template <class ..._ARGS> struct VALPacker<std::nullptr_t, _ARGS...> {
        template <class _T>
        static void pack(cocos2d::Vector<_T> &, std::nullptr_t, _ARGS...) {
        }
    };

    template <class _T0> struct VALPacker<_T0> {
        template <class _T>
        static void pack(cocos2d::Vector<_T> &vec, _T0 t0) {
            vec.pushBack(t0);
        }
    };

    template <> struct VALPacker<std::nullptr_t> {
        template <class _T>
        static void pack(cocos2d::Vector<_T> &, std::nullptr_t) {
        }
    };

    template <class _FINITE_TIME_ACTION_0, class ..._FINITE_TIME_ACTION_N>
    cocos2d::Sequence *createSequence(_FINITE_TIME_ACTION_0 action0, _FINITE_TIME_ACTION_N...actionN) {
        cocos2d::Vector<cocos2d::FiniteTimeAction *> vec;
        VALPacker<_FINITE_TIME_ACTION_0, _FINITE_TIME_ACTION_N...>::pack(vec, action0, actionN...);
        return cocos2d::Sequence::create(vec);
    }

    template <class _FINITE_TIME_ACTION_0, class ..._FINITE_TIME_ACTION_N>
    cocos2d::Spawn *createSpawn(_FINITE_TIME_ACTION_0 action0, _FINITE_TIME_ACTION_N...actionN) {
        cocos2d::Vector<cocos2d::FiniteTimeAction *> vec;
        VALPacker<_FINITE_TIME_ACTION_0, _FINITE_TIME_ACTION_N...>::pack(vec, action0, actionN...);
        return cocos2d::Spawn::create(vec);
    }
}

#endif
