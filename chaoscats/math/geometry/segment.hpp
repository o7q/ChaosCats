#pragma once

template <typename T>
struct Segment
{
    Segment() = default;
    Segment(Point2D<T> p1, Point2D<T> p2) : p1(p1), p2(p2) {}

    Point2D<T> p1, p2;
};