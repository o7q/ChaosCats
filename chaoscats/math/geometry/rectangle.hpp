#pragma once

#include "segment.hpp"
#include "vector.hpp"

template <typename T>
struct Rect
{
    Rect() = default;
    Rect(T x, T y, T w, T h)
    {
        this->position = Vector<T>(x, y);
        this->size = Vector<T>(w, h);
        half_size = Vector<T>(size.x / 2.0f, size.y / 2.0f);

        updatePoints();
    }

    void setPosition(T x, T y)
    {
        this->position = Vector<T>(x, y);
        updatePoints();
    }

    void incrementPosition(T x, T y)
    {
        this->position.x += x;
        this->position.y += y;
        updatePoints();
    }

    Vector<T> getPosition()
    {
        return position;
    }

    Vector<T> points[4];
    Segment<T> segments[4];

    Vector<T> size;
    Vector<T> half_size;

private:
    Vector<T> position;

    void updatePoints()
    {
        // 0   1
        //  [-]
        // 3   2

        points[0] = Vector<T>(position.x - half_size.x, position.y - half_size.y);
        points[1] = Vector<T>(position.x + half_size.x, position.y - half_size.y);
        points[2] = Vector<T>(position.x + half_size.x, position.y + half_size.y);
        points[3] = Vector<T>(position.x - half_size.x, position.y + half_size.y);

        segments[0] = Segment<T>(points[0], points[1]);
        segments[1] = Segment<T>(points[1], points[2]);
        segments[2] = Segment<T>(points[2], points[3]);
        segments[3] = Segment<T>(points[3], points[0]);
    }
};