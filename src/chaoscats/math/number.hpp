#pragma once

template <typename T>
int nsign(T value)
{
    return value > 0 ? 1 : -1;
}

inline float lerp(float v0, float v1, float t)
{
    return v0 + t * (v1 - v0);
}