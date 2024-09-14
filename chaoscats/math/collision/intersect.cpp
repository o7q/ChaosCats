#include "collision.hpp"
#include "../geometry/geometry_common.hpp"

#include <cmath>

// segment/segment intersection
// from: https://www.jeffreythompson.org/collision-detection/line-line.php
bool segmentSegment(Segment<float> a, Segment<float> b)
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

    return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
}

// segment/rectangle intersection
// from: https://www.jeffreythompson.org/collision-detection/line-rect.php
bool segmentRect(Segment<float> segment, Rect<float> rectangle)
{
    bool left = segmentSegment(segment, rectangle.segments[0]);
    bool top = segmentSegment(segment, rectangle.segments[1]);
    bool right = segmentSegment(segment, rectangle.segments[2]);
    bool bottom = segmentSegment(segment, rectangle.segments[3]);
    bool inPoint = pointRect(segment.p1, rectangle);

    return left || top || right || bottom || inPoint;
}

// point/rectangle intersection
int pointRect(Point2D<float> point, Rect<float> rect)
{
    const float half_width = rect.half_size.w;
    const float half_height = rect.half_size.h;
    const float dx = (point.x - (rect.position.x + rect.half_size.w));
    const float dy = (point.y - (rect.position.y + rect.half_size.h));
    const float abs_dx = std::fabs(dx);
    const float abs_dy = std::fabs(dy);

    if (abs_dx > half_width || abs_dy > half_height)
    {
        return 0;
    }

    const int tx = dx < 0 ? 3 : 4;
    const int ty = dy < 0 ? 1 : 2;

    return abs_dx * rect.size.h > abs_dy * rect.size.w ? tx : ty;
}

bool pointRect2(Point2D<float> endpoint, Rect<float> rect)
{
    // 0   1
    //  [-]
    // 3   2
    return endpoint.x > rect.points[0].x && endpoint.x < rect.points[1].x && endpoint.y > rect.points[0].y && endpoint.y < rect.points[1].y;
}