#pragma once

#include "../geometry/geometry_common.hpp"

namespace jmath
{
    // segment/segment intersection
    // from: https://www.jeffreythompson.org/collision-detection/line-line.php
    template <typename T>
    bool segmentSegment(jmath::Segment<T> a, jmath::Segment<T> b)
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
                   ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
        float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) /
                   ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

        if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
        {
            return true;
        }
        return false;
    }

    // segment/rectangle intersection
    // from: https://www.jeffreythompson.org/collision-detection/line-rect.php
    template <typename T>
    bool segmentRectangle(jmath::Segment<T> segment, jmath::Rectangle<T> rectangle)
    {
        bool left = segmentSegment(segment, rectangle.side_a);
        bool top = segmentSegment(segment, rectangle.side_b);
        bool right = segmentSegment(segment, rectangle.side_c);
        bool bottom = segmentSegment(segment, rectangle.side_d);

        if (left || top || right || bottom)
        {
            return true;
        }
        return false;
    }
}