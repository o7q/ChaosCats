#include "world.hpp"

void World::update(float dt)
{
    float speed = 1.0f / dt;
    for (float i = 0.0f; i < 1.0f; i += speed)
    {
        update_particles();
    }
    updateCollisionMesh(collisionMesh, particles, PlayerCollisionType::Solid, pixelSize);
}