#pragma once

template <typename T>
struct Point2D
{
    Point2D() = default;
    Point2D(T x, T y) : x(x), y(y) {}
    T x = 0, y = 0;
};

template <typename T>
struct Size2D
{
    Size2D() = default;
    Size2D(T w, T h) : w(w), h(h) {}
    T w = 0, h = 0;
};