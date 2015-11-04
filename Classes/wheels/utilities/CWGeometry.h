#ifndef _CW_GEOMETRY_H_
#define _CW_GEOMETRY_H_

#include "math/CCMath.h"
#include <vector>

namespace cw {
    namespace geometry {

        // 判断点在凸多边形内
        bool inConvexPolygon(const cocos2d::Vec2 vertices[], size_t n, const cocos2d::Vec2 &point);

        template <size_t _Size>
        inline bool inConvexPolygon(const cocos2d::Vec2 (&vertices)[_Size], const cocos2d::Vec2 &point) {
            return inConvexPolygon(vertices, _Size, point);
        }

        template <template <class> class _ALLOC>
        inline bool inConvexPolygon(const std::vector<cocos2d::Vec2, _ALLOC<cocos2d::Vec2> > &vertices, const cocos2d::Vec2 &point) {
            return inConvexPolygon(&vertices[0], vertices.size(), point);
        }

        // 判断点在线段上
        static inline bool onSegment(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2, const cocos2d::Vec2 &a) {
            if (p1.x < p2.x) { if (a.x < p1.x || a.x > p2.x) return false; }
            else             { if (a.x < p2.x || a.x > p1.x) return false; }

            if (p1.y < p2.y) { if (a.y < p1.y || a.y > p2.y) return false; }
            else             { if (a.y < p2.y || a.y > p1.y) return false; }

            return true;
        }

        // 判断线段相交
        bool isSegmentIntersect(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2, const cocos2d::Vec2 &p3, const cocos2d::Vec2 &p4);
    }
}

#endif
