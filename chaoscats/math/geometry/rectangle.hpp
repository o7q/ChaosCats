#pragma once

#include "segment.hpp"
#include "point.hpp"

template <typename T>
struct Rect
{
    Rect() = default;
    Rect(T x, T y, T w, T h)
    {
        this->position = Point<T>(x, y);
        this->size = Size<T>(w, h);
        half_size = Size<T>(size.w / 2.0f, size.h / 2.0f);

        updatePoints();
    }

    void setPosition(T x, T y)
    {
        this->position = Point<T>(x, y);
        updatePoints();
    }

    void incrementPosition(T x, T y)
    {
        this->position.x += x;
        this->position.y += y;
        updatePoints();
    }

    Point<T> getPosition()
    {
        return position;
    }

    Point<T> points[4];
    Segment<T> segments[4];

    Size<T> size;
    Size<T> half_size;

private:
    Point<T> position;

    void updatePoints()
    {
        // 0   1
        //  [-]
        // 3   2

        points[0] = Point<T>(position.x - half_size.w, position.y - half_size.h);
        points[1] = Point<T>(position.x + half_size.w, position.y - half_size.h);
        points[2] = Point<T>(position.x + half_size.w, position.y + half_size.h);
        points[3] = Point<T>(position.x - half_size.w, position.y + half_size.h);

        segments[0] = Segment<T>(points[0], points[1]);
        segments[1] = Segment<T>(points[1], points[2]);
        segments[2] = Segment<T>(points[2], points[3]);
        segments[3] = Segment<T>(points[3], points[0]);
    }
};