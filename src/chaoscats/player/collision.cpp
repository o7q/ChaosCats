#include "player.hpp"

void Player::handleCollision()
{
    std::vector<EdgeData> collisionMesh = world->getCollisionMesh();
    bool collisionHappened = false;
    for (EdgeData edge : collisionMesh)
    {
        bool bottomCollision = handleBottomCollision(position, velocity, player_collisionBox, edge, bottomCollisionPoint, gameInfo.time.DELTA_TIME);
        bool topCollision = handleTopCollision(position, velocity, player_collisionBox, edge, topCollisionPoint, gameInfo.time.DELTA_TIME);

        if (bottomCollision || topCollision)
        {
            collisionHappened = true;
        }
    }

    if (!collisionHappened)
    {
        position.x += velocity.x * gameInfo.time.DELTA_TIME;
        position.y += velocity.y * gameInfo.time.DELTA_TIME;
    }
}