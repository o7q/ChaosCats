#pragma once

#include <iostream>
#include <vector>

template <typename T>
struct Segment
{
    Segment() = default;
    Segment(Vector<T> p1, Vector<T> p2) : p1(p1), p2(p2) {}

    Vector<T> p1, p2;
};

template <typename T>
std::vector<Segment<T>> segmentPathFromPoints(std::vector<Vector<T>> &points)
{
    std::vector<Segment<T>> segmentPath;
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        Segment<T> segment(points[i], points[i + 1]);
        segmentPath.push_back(segment);
    }

    return segmentPath;
}