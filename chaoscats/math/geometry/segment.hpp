#pragma once

template <typename T>
struct Segment
{
    Segment() = default;
    Segment(Point<T> p1, Point<T> p2) : p1(p1), p2(p2) {}

    Point<T> p1, p2;
};