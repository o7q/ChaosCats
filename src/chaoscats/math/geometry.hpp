#pragma once

#include <cmath>

#include "types/types_common.hpp"

template <typename T>
float distance(Vector<T> a, Vector<T> b)
{
    return std::sqrt(
        std::pow(a.x - b.x, 2) +
        std::pow(a.y - b.y, 2));
}