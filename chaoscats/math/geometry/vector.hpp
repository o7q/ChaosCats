#pragma once

template <typename T>
struct Vector
{
    Vector() = default;
    Vector(T x, T y) : x(x), y(y) {}
    T x = 0, y = 0;
};