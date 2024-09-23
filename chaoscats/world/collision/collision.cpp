#include "collision.hpp"

#include <iostream>
#include <cmath>

bool handleBottomCollision(Point<float> &position, Point<float> &velocity, Rect<float> collisionBox, EdgeData edge, Point<float> bottom, float dt)
{
    Point<float> playerFuturePos;
    playerFuturePos.x = bottom.x + velocity.x * dt;
    playerFuturePos.y = bottom.y + velocity.y * dt;

    Segment<float> futureVelocity = Segment<float>(bottom, playerFuturePos);

    SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);

    Point<float> intersectPoint = Point<float>(intersectData.x, intersectData.y);

    // check if the edge intersected
    if (!intersectData.intersected)
    {
        return false;
    }

    // handle floors
    if (edge.slopeType == SlopeType::Horizontal)
    {
        position.x = intersectPoint.x;
        position.y = intersectPoint.y - collisionBox.half_size.h - 1;

        return true;
    }

    // handle slopes
    if (edge.slopeType == SlopeType::UpHill ||
        edge.slopeType == SlopeType::DownHill)
    {
        position.x = intersectPoint.x;
        position.y = intersectPoint.y - collisionBox.half_size.h - std::fabs(velocity.x * dt) - 1;

        return true;
    }

    // handle walls
    if (edge.slopeType == SlopeType::Vertical)
    {
        int sign = velocity.x >= 0 ? 1 : -1;

        position.x = intersectPoint.x - sign * 2;
        position.y = intersectPoint.y - collisionBox.half_size.h;

        return true;
    }

    return false;
}

bool handleTopCollision(Point<float> &position, Point<float> &velocity, Rect<float> collisionBox, EdgeData edge, Point<float> top, float dt)
{
    Point<float> playerFuturePos;
    playerFuturePos.x = top.x + velocity.x * dt;
    playerFuturePos.y = top.y + velocity.y * dt;

    Segment<float> futureVelocity = Segment<float>(top, playerFuturePos);

    SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);

    Point<float> intersectPoint = Point<float>(intersectData.x, intersectData.y);

    // check if the edge intersected
    if (!intersectData.intersected)
    {
        return false;
    }

    // handle ceilings
    if (edge.slopeType == SlopeType::Horizontal)
    {
        position.x = intersectPoint.x;
        position.y = intersectPoint.y + collisionBox.half_size.h + 1;

        velocity.y = 0;

        return true;
    }

    // handle sloped ceilings
    if (edge.slopeType == SlopeType::UpHill ||
        edge.slopeType == SlopeType::DownHill)
    {
        int sign = velocity.x >= 0 ? 1 : -1;

        position.x = intersectPoint.x - sign * 2;
        position.y = intersectPoint.y + collisionBox.half_size.h;

        velocity.y = 0;

        return true;
    }

    // handle walls
    if (edge.slopeType == SlopeType::Vertical)
    {
        int sign = velocity.x >= 0 ? 1 : -1;

        position.x = intersectPoint.x - sign * 2;
        position.y = intersectPoint.y + collisionBox.half_size.h;

        return true;
    }

    return false;
}