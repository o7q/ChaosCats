#pragma once

#include "segment.hpp"
#include "point.hpp"

template <typename T>
struct Rect
{
    Rect() = default;
    Rect(Point2D<T> topLeft, Size2D<T> size)
    {
        init(topLeft, {topLeft.x + size.w, topLeft.y + size.h});
    }
    Rect(Point2D<T> topLeft, Point2D<T> bottomRight)
    {
        init(topLeft, bottomRight);
    }

    Point2D<T> points[4];

    Point2D<T> position;

    Size2D<T> size;
    Size2D<T> half_size;

private:
    void init(Point2D<T> topLeft, Point2D<T> bottomRight)
    {
        size.w = bottomRight.x - topLeft.x;
        size.h = bottomRight.y - topLeft.y;

        half_size.w = size.w / 2.0f;
        half_size.h = size.h / 2.0f;

        position = topLeft;

        // 1   2
        //  [-]
        // 4   3

        points[0] = topLeft;
        points[1] = Point2D<T>(topLeft.x + size.w, topLeft.y);
        points[2] = Point2D<T>(topLeft.x + size.w, topLeft.y + size.h);
        points[3] = Point2D<T>(topLeft.x, topLeft.y + size.h);
    }
};