#pragma once

template <typename T>
int nsign(T value)
{
    return value > 0 ? 1 : -1;
}