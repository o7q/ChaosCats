#include "entity.hpp"

void Entity::moveUpdate(bool checkWorldCollisions)
{
    if (world != nullptr && checkWorldCollisions)
    {
        std::vector<EdgeData> collisionMesh = world->getCollisionMesh();
        for (EdgeData edge : collisionMesh)
        {
            solveWorldCollision(collisionSegment, edge, boundingBox, position, velocity, gameState.time.DELTA_TIME);
        }
    }

    position.x += velocity.x * gameState.time.DELTA_TIME;
    position.y += velocity.y * gameState.time.DELTA_TIME;

    updateChildPositions();
}