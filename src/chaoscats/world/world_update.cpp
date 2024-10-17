#include "world.hpp"

void World::update(float dt, float base_dt)
{
    float speed_divider = 0.25f;

    float inverse_dt = speed_divider / dt;

    if (dt > speed_divider || dt > base_dt)
    {
        for (float i = 0.0f; i < speed_divider; i += inverse_dt)
        {
            update_particles();
            updateCollisionMesh(collisionMesh, particles, PlayerCollisionType::Solid, pixelSize);
        }
    }
    else
    {
        if (update_counter > speed_divider)
        {
            update_counter = 0.0f;
            update_particles();
            updateCollisionMesh(collisionMesh, particles, PlayerCollisionType::Solid, pixelSize);
        }

        update_counter += dt;
    }
}