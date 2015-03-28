﻿#ifndef _CW_GEOMETRY_H_
#define _CW_GEOMETRY_H_

#include "math/CCMath.h"
#include <vector>

namespace cw {
    namespace geometry {
        using cocos2d::Vec2;

        // 判断点在凸多边形内
        bool inConvexPolygon(const Vec2 vertices[], size_t n, const Vec2 &point);

        template <size_t _N>
        inline bool inConvexPolygon(const Vec2 (&vertices)[_N], const Vec2 &point) {
            return inConvexPolygon(vertices, _N, point);
        }

        template <template <class> class _ALLOC>
        inline bool inConvexPolygon(const std::vector<Vec2, _ALLOC<Vec2> > &vertices, const Vec2 &point) {
            return inConvexPolygon(&vertices[0], vertices.size(), point);
        }

        // 判断点在线段上
        static inline bool onSegment(const Vec2 &p1, const Vec2 &p2, const Vec2 &a) {
            if (p1.x < p2.x) { if (a.x < p1.x || a.x > p2.x) return false; }
            else             { if (a.x < p2.x || a.x > p1.x) return false; }

            if (p1.y < p2.y) { if (a.y < p1.y || a.y > p2.y) return false; }
            else             { if (a.y < p2.y || a.y > p1.y) return false; }

            return true;
        }

        // 判断线段相交
        bool isSegmentIntersect(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4);
    }
}

#endif
