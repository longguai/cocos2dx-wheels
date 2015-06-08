#ifndef _CW_LAYOUT_FUNCTIONS_H_
#define _CW_LAYOUT_FUNCTIONS_H_

#include "math/Vec2.h"
#include "math/CCGeometry.h"

namespace cw {

    // 将一组NODE排成一个矩阵
    template <class _NODE, class _VECTOR>
    _NODE *arrangeAsMatrixByColumn(const _VECTOR &array, int col, const cocos2d::Size &gapSize, cocos2d::Size eachContentSize = cocos2d::Size(0.0f, 0.0f)) {
        using namespace cocos2d;

        _NODE *root = _NODE::create();
        int cnt = array.size();
        if (cnt == 0) {
            return root;
        }

        div_t ret = div(cnt, col);
        int row = ret.quot + (ret.rem > 0 ? 1 : 0);

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
}

#endif
