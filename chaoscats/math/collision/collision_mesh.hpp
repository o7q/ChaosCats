#pragma once

#include <vector>

#include "../geometry/geometry_common.hpp"
#include "../matrix/matrix.hpp"

template <typename T>
std::vector<Segment<T>> segmentPathFromPoints(std::vector<Point<T>> &points)
{
    std::vector<Segment<T>> segmentPath;
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        Segment<T> segment(points[i], points[i + 1]);
        segmentPath.push_back(segment);
    }

    return segmentPath;
}

std::vector<Point<int>> pointsFromHollowBinaryMatrix(Matrix2D<int> &hollowBinaryMatrix);