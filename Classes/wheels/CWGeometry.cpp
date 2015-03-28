#include "CWGeometry.h"

namespace cw {
    namespace geometry {
        bool inConvexPolygon(const Vec2 vertices[], size_t n, const Vec2 &point) {
            CC_ASSERT(n > 2);
            --n;

            Vec2 pn;
            Vec2::subtract(vertices[n], point, &pn);
            Vec2 n0;
            Vec2::subtract(vertices[0], vertices[n], &n0);
            float cross = pn.cross(n0);
            bool positive = true;
            if (cross == 0) {
                return true;
            } else if (cross < 0) {
                positive = false;
            }

            for (size_t i = 0; i < n; ++i) {
                Vec2 pi0;
                Vec2::subtract(vertices[i], point, &pi0);
                Vec2 i1i0;
                Vec2::subtract(vertices[i + 1], vertices[i], &i1i0);
                cross = pi0.cross(i1i0);
                if (cross == 0) {
                    return true;
                } else if (cross < 0) {
                    if (positive) {
                        return false;
                    }
                } else {
                    if (!positive) {
                        return false;
                    }
                }
            }

            return true;
        }

        bool isSegmentIntersect(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4) {
            if ((p1.x == p2.x && p1.y == p2.y) || (p3.x == p4.x && p3.y == p4.y)) {
                return false;
            }

            const float x13 = p3.x - p1.x;
            const float y13 = p3.y - p1.y;
            const float x14 = p4.x - p1.x;
            const float y14 = p4.y - p1.y;

            const float x23 = p3.x - p2.x;
            const float y23 = p3.y - p2.y;
            const float x43 = p3.x - p4.x;
            const float y43 = p3.y - p4.y;

            //const float x24 = p4.x - p2.x;
            //const float y24 = p4.y - p2.y;

            const float x31 = -x13;
            const float y31 = -x13;
            const float x21 = p1.x - p2.x;
            const float y21 = p1.y - p2.y;

            const float x41 = -x14;
            const float y41 = -y14;

            const float d1 = x13 * y14 - y13 * x14;
            const float d2 = x23 * y43 - y23 * x43;
            const float d3 = x31 * y21 - y31 * x21;
            const float d4 = x41 * y21 - y41 * x21;

            if (((d1 > 0.0F && d2 < 0.0F) || (d1 < 0.0F && d2 > 0.0F)) && ((d3 > 0.0F && d4 < 0.0F) || (d3 < 0.0F && d4 > 0.0F))) {
                return true;
            } else if (d1 == 0.0F && onSegment(p3, p4, p1)) {
                return true;
            } else if (d2 == 0.0F && onSegment(p3, p4, p1)) {
                return true;
            } else if (d3 == 0.0F && onSegment(p1, p2, p3)) {
                return true;
            } else if (d4 == 0.0F && onSegment(p1, p2, p4)) {
                return true;
            }

            return false;
        }
    }
}
