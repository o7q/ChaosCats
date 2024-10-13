#include "player.hpp"

#include "../math/collision/collision.hpp"

void Player::handleCollision()
{
    std::vector<EdgeData> collisionMesh = world->getCollisionMesh();
    for (EdgeData edge : collisionMesh)
    {
        solveWorldCollision(collisionSegment, edge, player_collisionBox, position, velocity, gameInfo.time.DELTA_TIME);
    }

    position.x += velocity.x * gameInfo.time.DELTA_TIME;
    position.y += velocity.y * gameInfo.time.DELTA_TIME;
}