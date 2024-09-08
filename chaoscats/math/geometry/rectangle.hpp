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

        Segment<T> side_a, side_b, side_c, side_d;

        Point<T> position;

        Size<T> size;

    private:
        void init(Point<T> topLeft, Point<T> bottomRight)
        {
            size.w = bottomRight.x - topLeft.x;
            size.h = bottomRight.y - topLeft.y;

            position = topLeft;

            //    B
            // A [-] C
            //    D

            side_a = jmath::Segment(
                jmath::Point(topLeft.x, topLeft.y + size.h),
                jmath::Point(topLeft.x, topLeft.y));
            side_b = jmath::Segment(
                jmath::Point(topLeft.x, topLeft.y),
                jmath::Point(topLeft.x + size.w, topLeft.y));
            side_c = jmath::Segment(
                jmath::Point(topLeft.x + size.w, topLeft.y),
                jmath::Point(bottomRight.x, bottomRight.y));
            side_d = jmath::Segment(
                jmath::Point(bottomRight.x, bottomRight.y),
                jmath::Point(bottomRight.x - size.w, bottomRight.y));
        }
    };
}