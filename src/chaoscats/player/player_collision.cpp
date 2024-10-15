#include "player.hpp"

#include "../math/collision/collision.hpp"

void Player::handleCollision()
{
    std::vector<EdgeData> collisionMesh = world->getCollisionMesh();
    for (EdgeData edge : collisionMesh)
    {
        solveWorldCollision(collisionSegment, edge, player_collisionBox, position, velocity, gameState.time.DELTA_TIME);
    }

    position.x += velocity.x * gameState.time.DELTA_TIME;
    position.y += velocity.y * gameState.time.DELTA_TIME;
}