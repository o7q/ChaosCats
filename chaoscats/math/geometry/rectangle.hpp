#pragma once

#include "segment.hpp"

namespace jmath
{
    namespace geo
    {
        template <typename T>
        struct Rectangle
        {
            Rectangle(Point<T> topLeft, Size<T> size)
            {
                init(topLeft, {topLeft.x + size.w, topLeft.y + size.h});
            }
            Rectangle(Point<T> topLeft, Point<T> bottomRight)
            {
                init(topLeft, bottomRight);
            }

            Segment<T> side_a{0, 0}, side_b{0, 0}, side_c{0, 0}, side_d{0, 0};

            Size<T> size;
            Size<T> half_size;

            Point<T> topLeft;
            Point<T> bottomRight;

        private:
            void init(Point<T> topLeft, Point<T> bottomRight)
            {
                size.w = bottomRight.x - topLeft.x;
                size.h = bottomRight.y - topLeft.y;

                half_size.w = size.w / 2;
                half_size.h = size.h / 2;

                this->topLeft = topLeft;
                this->bottomRight = bottomRight;

                //    B
                // A [-] C
                //    D

                side_a = {topLeft.x, topLeft.y + size.h, topLeft.x, topLeft.y};
                side_b = {topLeft.x, topLeft.y, topLeft.x + size.w, topLeft.y};
                side_c = {topLeft.x + size.w, topLeft.y, bottomRight.x, bottomRight.y};
                side_d = {bottomRight.x, bottomRight.y, bottomRight.x - size.w, bottomRight.y};
            }
        };
    }
}