#pragma once

#include "../../math/geometry/geometry_common.hpp"
#include "collision_mesh.hpp"

bool handleBottomCollision(Rect<float> &rect, Point<float> velocity, EdgeData edge, Point<float> bottom, float dt);
bool handleTopCollision(Rect<float> &rect, Point<float> velocity, EdgeData edge, Point<float> top, float dt);

struct SS_IntersectData
{
    float intersected = false;
    float uA = 0;
    float uB = 0;
    float x = 0;
    float y = 0;
};

SS_IntersectData segmentSegment(Segment<float> a, Segment<float> b);