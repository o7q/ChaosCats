#pragma once

#include <vector>

#include "../geometry/geometry_common.hpp"

template <typename T>
std::vector<jmath::Segment<T>> segmentsFromPoints(std::vector<jmath::Point<T>> points)
{
    std::vector<jmath::Segment<T>> segments;
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        jmath::Segment<T> segment(points[i], points[i + 1]);
        segments.push_back(segment);
    }

    return segments;
}