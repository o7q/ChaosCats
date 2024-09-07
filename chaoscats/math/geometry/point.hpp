#pragma once

namespace jmath
{
    namespace geo
    {
        template <typename T>
        struct Point
        {
            T x = 0, y = 0;
        };

        template <typename T>
        struct Size
        {
            T w = 0, h = 0;
        };
    }
}