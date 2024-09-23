#include "collision.hpp"

#include <iostream>
#include <cmath>

bool handleBottomCollision(Rect<float> &rect, Point<float> velocity, EdgeData edge, Point<float> bottom, float dt)
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
        rect.setPosition(intersectPoint.x, intersectPoint.y - rect.half_size.h);

        return true;
    }

    // handle slopes
    if (edge.slopeType == SlopeType::UpHill || edge.slopeType == SlopeType::DownHill)
    {
        rect.setPosition(intersectPoint.x, intersectPoint.y - rect.half_size.h - std::fabs(velocity.x * dt));

        return true;
    }

    // handle walls, NOT FULLY WORKING YET!!
    // if (edge.slope >= 1.1f || edge.slope <= -1.1f)
    // {
    //     int sign = velocity.x >= 0 ? 1 : -1;

    //     rect.setPosition(intersectPoint.x - sign * 10, intersectPoint.y - rect.half_size.h);

    //     return;
    // }

    return false;
}

bool handleTopCollision(Rect<float> &rect, Point<float> velocity, EdgeData edge, Point<float> top, float dt)
{
    Point<float> playerFuturePos;
    playerFuturePos.x = top.x + velocity.x * dt;
    playerFuturePos.y = top.y + velocity.y * dt;

    Segment<float> futureVelocity = Segment<float>(top, playerFuturePos);

    SS_IntersectData idata = segmentSegment(futureVelocity, edge.segment);

    Point<float> intersectPoint = Point<float>(idata.x, idata.y);

    // check if the edge intersected
    if (!idata.intersected)
    {
        return false;
    }

    // handle ceilings
    if (edge.slope <= 1.1f && edge.slope >= -1.1f)
    {
        rect.setPosition(intersectPoint.x, intersectPoint.y + rect.half_size.h);

        return true;
    }

    return false;
}