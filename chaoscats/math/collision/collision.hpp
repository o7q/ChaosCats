#pragma once

#include "../geometry/geometry_common.hpp"

enum class CollisionSide
{
    Left,
    Right,
    Top,
    Bottom,
    None
};

struct SS_OverlapData
{
    float x = 0.0f;
    float y = 0.0f;
    bool overlap = false;
};

bool segmentSegment(Segment<float> a, Segment<float> b);
bool segmentRect(Segment<float> segment, Rect<float> rectangle);
int pointRect(Point2D<float> point, Rect<float> rect);
bool pointRect2(Point2D<float> endpoint, Rect<float> rect);

float getTimeToCollision(Rect<float> rect, std::vector<Segment<float>> &segments, float maxDt, float minDt);
void simulationStep(Rect<float> rect, std::vector<Segment<float>> &segments, float dt, float vx, float vy, float &x, float &y);
bool isRectColliding(Rect<float> rect, std::vector<Segment<float>> segments);