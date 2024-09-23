#pragma once

#include "../../math/geometry/geometry_common.hpp"
#include "collision_mesh.hpp"

bool handleBottomCollision(Point<float> &position, Point<float> &velocity, Rect<float> collisionBox, EdgeData edge, Point<float> bottom, float dt);
bool handleTopCollision(Point<float> &position, Point<float> &velocity, Rect<float> collisionBox, EdgeData edge, Point<float> top, float dt);

struct SS_IntersectData
{
    float intersected = false;
    float uA = 0;
    float uB = 0;
    float x = 0;
    float y = 0;
};

SS_IntersectData segmentSegment(Segment<float> a, Segment<float> b);