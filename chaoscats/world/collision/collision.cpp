#include "collision.hpp"

#include <iostream>
#include <cmath>

bool handleBottomCollision(Point<float> &position, Point<float> velocity, Rect<float> collisionBox, EdgeData edge, Point<float> bottom, float dt)
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
        position.y = intersectPoint.y - collisionBox.half_size.h;

        return true;
    }

    // handle slopes
    if (edge.slopeType == SlopeType::UpHill || edge.slopeType == SlopeType::DownHill)
    {
        position.x = intersectPoint.x;
        position.y = intersectPoint.y - collisionBox.half_size.h - std::fabs(velocity.x * dt);

        return true;
    }

    // handle walls, NOT FULLY WORKING YET!!
    if (edge.slopeType == SlopeType::Vertical)
    {
        int sign = velocity.x >= 0 ? 1 : -1;

        position.x = intersectPoint.x - sign;
        position.y = intersectPoint.y - collisionBox.half_size.h;

        return true;
    }

    return false;
}