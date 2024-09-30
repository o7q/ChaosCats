#include "player.hpp"

void Player::handleCollision()
{
    std::vector<EdgeData> collisionMesh = world->getCollisionMesh();
    bool collisionHappened = false;
    for (EdgeData edge : collisionMesh)
    {
        bool bottomCollision = handleBottomCollision(playerData.position, velocity, collisionBox, edge, bottomCollisionPoint, windowInfo.DELTA_TIME);
        bool topCollision = handleTopCollision(playerData.position, velocity, collisionBox, edge, topCollisionPoint, windowInfo.DELTA_TIME);

        if (bottomCollision || topCollision)
        {
            collisionHappened = true;
        }
    }

    if (!collisionHappened)
    {
        playerData.position.y += velocity.y * windowInfo.DELTA_TIME;
        playerData.position.x += velocity.x * windowInfo.DELTA_TIME;
    }
}