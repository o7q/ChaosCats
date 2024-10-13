#pragma once

#include "geometry/geometry_common.hpp"

struct SS_IntersectData
{
    float intersected = false;
    float uA = 0;
    float uB = 0;
    float x = 0;
    float y = 0;
};

// segment/segment intersection
// from: https://www.jeffreythompson.org/collision-detection/line-line.php
inline SS_IntersectData segmentSegment(Segment<float> a, Segment<float> b)
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

    SS_IntersectData intersect_data;
    intersect_data.intersected = uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
    intersect_data.uA = uA;
    intersect_data.uB = uB;
    intersect_data.x = x1 + (uA * (x2 - x1));
    intersect_data.y = y1 + (uA * (y2 - y1));

    return intersect_data;
}

// point in poly detection
// from: https://stackoverflow.com/a/218081
inline bool pointInPoly(Vector<float> point, std::vector<Segment<float>> &poly, Vector<float> origin)
{
    int intersects = 0;
    const Segment<float> testSegment(origin, point);
    for (const Segment<float> &segment : poly)
    {
        if (segmentSegment(segment, testSegment).intersected)
        {
            ++intersects;
        }
    }

    return intersects % 2 == 1;
}