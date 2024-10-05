#pragma once

#include <cmath>

#include "geometry/geometry_common.hpp"
#include "constants.hpp"

inline float radiansToDegrees(float radians)
{
    float degrees = (180.0f / PI) * radians;
    return std::fmod(degrees + 360.0f, 360.0f);
}

inline Point<float> rotatePoint(Point<float> point, float angle)
{
    Point<float> newPoint;
    newPoint.x = point.x * std::cos(angle) - point.y * std::sin(angle);
    newPoint.y = point.x * std::sin(angle) + point.y * std::cos(angle);

    return newPoint;
}

inline float radiansFromPoints(float startX, float startY, float endX, float endY)
{
    float dx = endX - startX;
    float dy = endY - startY;
    float radians = std::atan2(dy, dx);

    return radians;
}

inline float degreesFromPoints(float startX, float startY, float endX, float endY)
{
    float dx = endX - startX;
    float dy = endY - startY;
    float radians = std::atan2(dy, dx);

    return radiansToDegrees(radians);
}