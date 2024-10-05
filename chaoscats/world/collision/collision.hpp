#pragma once

#include "../../math/geometry/geometry_common.hpp"
#include "collision_mesh.hpp"

bool handleBottomCollision(Vector<float> &position, Vector<float> &velocity, Rect<float> collisionBox, EdgeData edge, Vector<float> bottom, float dt);
bool handleTopCollision(Vector<float> &position, Vector<float> &velocity, Rect<float> collisionBox, EdgeData edge, Vector<float> top, float dt);

struct SS_IntersectData
{
    float intersected = false;
    float uA = 0;
    float uB = 0;
    float x = 0;
    float y = 0;
};

SS_IntersectData segmentSegment(Segment<float> a, Segment<float> b);