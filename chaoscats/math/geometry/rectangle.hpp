#pragma once

#include "segment.hpp"
#include "point.hpp"

namespace jmath
{
    template <typename T>
    struct Rectangle
    {
        Rectangle() = default;
        Rectangle(Point<T> topLeft, Size<T> size)
        {
            init(topLeft, {topLeft.x + size.w, topLeft.y + size.h});
        }
        Rectangle(Point<T> topLeft, Point<T> bottomRight)
        {
            init(topLeft, bottomRight);
        }

        Point<T> points[4];

        Point<T> position;

        Size<T> size;
        Size<T> half_size;

    private:
        void init(Point<T> topLeft, Point<T> bottomRight)
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
            points[1] = jmath::Point<T>(topLeft.x + size.w, topLeft.y);
            points[2] = jmath::Point<T>(topLeft.x + size.w, topLeft.y + size.h);
            points[3] = jmath::Point<T>(topLeft.x, topLeft.y + size.h);
        }
    };
}