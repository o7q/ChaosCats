#include "collision.hpp"

float distancePointToSegment(Point2D<float> point, Point2D<float> segPoint1, Point2D<float> segPoint2)
{
    float x0 = point.x;
    float x1 = segPoint1.x;
    float x2 = segPoint2.x;

    float y0 = point.y;
    float y1 = segPoint1.y;
    float y2 = segPoint2.y;

    float n = std::abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1);
    float d = std::sqrt(std::pow(y2 - y1, 2) + std::pow(x2 - x1, 2));

    return n / d;
}

float minDistanceFromRectToSegment(const Rect<float> &rect, const Segment<float> &seg)
{
    // float minDist = std::numeric_limits<float>::max();

    // // Check distance from rectangle's corners to the segment
    // for (float x = 0; x <= rect.size.w; x += rect.size.w)
    // {
    //     for (float y = 0; y <= rect.size.h; y += rect.size.h)
    //     {
    //         Point<float> corner(rect.position.x + x, rect.position.y + y);
    //         float dist = distancePointToSegment(corner, seg.p1, seg.p2);
    //         minDist = std::min(minDist, dist);
    //     }
    // }

    return 1.0f;
    ;
}