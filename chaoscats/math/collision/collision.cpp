#include "collision.hpp"

namespace jmath
{
    namespace col
    {
        // segment/segment intersection
        // from: https://www.jeffreythompson.org/collision-detection/line-line.php
        bool segmentSegment(jmath::geo::Segment<float> a, jmath::geo::Segment<float> b)
        {
            float uA = ((b.x2 - b.x) * (a.y - b.y) - (b.y2 - b.y) * (a.x - b.x)) /
                       ((b.y2 - b.y) * (a.x2 - a.x) - (b.x2 - b.x) * (a.y2 - a.y));

            float uB = ((a.x2 - a.x) * (a.y - b.y) - (a.y2 - a.y) * (a.x - b.x)) /
                       ((b.y2 - b.y) * (a.x2 - a.x) - (b.x2 - b.x) * (a.y2 - a.y));

            if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
            {
                return true;
            }
            return false;
        }

        bool segmentRectangle(jmath::geo::Segment<float> segment, jmath::geo::Rectangle<float> rectangle)
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
}