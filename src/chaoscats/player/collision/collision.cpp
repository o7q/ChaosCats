#include <iostream>
#include <cmath>

#include "../../math/intersection.hpp"
#include "../../world/collision_mesh/collision_mesh.hpp"

#include "../player.hpp"

bool Player::handleBottomCollision(Vector<float> &position, Vector<float> &velocity, Rect<float> collisionBox, EdgeData edge, Vector<float> bottom, float dt)
{
    Vector<float> playerFuturePos;
    playerFuturePos.x = bottom.x + velocity.x * dt;
    playerFuturePos.y = bottom.y + velocity.y * dt;

    Segment<float> futureVelocity = Segment<float>(bottom, playerFuturePos);

    SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);

    Vector<float> intersectPoint = Vector<float>(intersectData.x, intersectData.y);

    // check if the edge intersected
    if (!intersectData.intersected)
    {
        return false;
    }

    // handle floors
    if (edge.slopeType == SlopeType::Horizontal)
    {
        position.x = intersectPoint.x;
        position.y = intersectPoint.y - collisionBox.half_size.y - 1;

        return true;
    }

    // handle slopes
    if (edge.slopeType == SlopeType::UpHill ||
        edge.slopeType == SlopeType::DownHill)
    {
        position.x = intersectPoint.x;
        position.y = intersectPoint.y - collisionBox.half_size.y - std::fabs(velocity.x * dt) - 1;

        return true;
    }

    // handle walls
    if (edge.slopeType == SlopeType::Vertical)
    {
        int sign = velocity.x >= 0 ? 1 : -1;

        position.x = intersectPoint.x - sign * 2;
        position.y = intersectPoint.y - collisionBox.half_size.y;

        return true;
    }

    return false;
}

bool Player::handleTopCollision(Vector<float> &position, Vector<float> &velocity, Rect<float> collisionBox, EdgeData edge, Vector<float> top, float dt)
{
    Vector<float> playerFuturePos;
    playerFuturePos.x = top.x + velocity.x * dt;
    playerFuturePos.y = top.y + velocity.y * dt;

    Segment<float> futureVelocity = Segment<float>(top, playerFuturePos);

    SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);

    Vector<float> intersectPoint = Vector<float>(intersectData.x, intersectData.y);

    // check if the edge intersected
    if (!intersectData.intersected)
    {
        return false;
    }

    // handle ceilings
    if (edge.slopeType == SlopeType::Horizontal)
    {
        position.x = intersectPoint.x;
        position.y = intersectPoint.y + collisionBox.half_size.y + 1;

        velocity.y = 0;

        return true;
    }

    // handle sloped ceilings
    if (edge.slopeType == SlopeType::UpHill ||
        edge.slopeType == SlopeType::DownHill)
    {
        int sign = velocity.x >= 0 ? 1 : -1;

        position.x = intersectPoint.x - sign * 2;
        position.y = intersectPoint.y + collisionBox.half_size.y;

        velocity.y = 0;

        return true;
    }

    // handle walls
    if (edge.slopeType == SlopeType::Vertical)
    {
        int sign = velocity.x >= 0 ? 1 : -1;

        position.x = intersectPoint.x - sign * 2;
        position.y = intersectPoint.y + collisionBox.half_size.y;

        return true;
    }

    return false;
}