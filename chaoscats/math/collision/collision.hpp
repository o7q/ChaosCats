#pragma once

#include <cmath>

#include "../geometry/geometry_common.hpp"

namespace jmath
{
    enum class CollisionSide
    {
        Left,
        Right,
        Top,
        Bottom,
        None
    };

    struct SS_OverlapData
    {
        bool overlapping = false;
        float uA = 0;
        float uB = 0;
    };

    struct SR_OverlapData : public SS_OverlapData
    {
        CollisionSide collisionSide = CollisionSide::None;
    };

    // segment/segment intersection
    // from: https://www.jeffreythompson.org/collision-detection/line-line.php
    template <typename T>
    SS_OverlapData segmentSegment(Segment<T> a, Segment<T> b)
    {
        float x1 = a.p1.x;
        float x2 = a.p2.x;
        float x3 = b.p1.x;
        float x4 = b.p2.x;

        float y1 = a.p1.y;
        float y2 = a.p2.y;
        float y3 = b.p1.y;
        float y4 = b.p2.y;

        float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) /
                   static_cast<float>((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
        float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) /
                   static_cast<float>((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

        if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
        {
            return SS_OverlapData{true, uA, uB};
        }
        return SS_OverlapData();
    }

    // segment/rectangle intersection
    // from: https://www.jeffreythompson.org/collision-detection/line-rect.php
    template <typename T>
    SR_OverlapData segmentRect(Segment<T> segment, Rect<T> rectangle)
    {
        // SS_OverlapData left = segmentSegment(segment, rectangle.side_a);
        // SS_OverlapData top = segmentSegment(segment, rectangle.side_b);
        // SS_OverlapData right = segmentSegment(segment, rectangle.side_c);
        // SS_OverlapData bottom = segmentSegment(segment, rectangle.side_d);

        // if (left.overlapping)
        // {
        //     return SR_OverlapData{left, CollisionSide::Left};
        // }
        // if (top.overlapping)
        // {
        //     return SR_OverlapData{top, CollisionSide::Top};
        // }
        // if (right.overlapping)
        // {
        //     return SR_OverlapData{right, CollisionSide::Right};
        // }
        // if (bottom.overlapping)
        // {
        //     return SR_OverlapData{bottom, CollisionSide::Bottom};
        // }

        return SR_OverlapData();
    }

    // point to segment distance
    // from: https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
    float distancePointToSegment(Point2D<float> point, Point2D<float> segPoint1, Point2D<float> segPoint2);

    float minDistanceFromRectToSegment(const Rect<float> &rect, const Segment<float> &seg);
}