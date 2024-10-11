#include "world.hpp"

void World::update(float dt)
{
    for (int i = 0; i < static_cast<int>(std::ceil(dt)); ++i)
    {
        update_particles();
    }
    updateCollisionMesh(collisionMesh, particles, PlayerCollisionType::Solid, pixelSize);
}