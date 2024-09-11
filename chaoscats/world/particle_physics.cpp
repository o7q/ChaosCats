#include "world.hpp"

#include "../math/random/random.hpp"

void World::particles_update()
{
    auto updateParticle = [this](int x, int y)
    {
        Particle particle = particles.get(y, x);
        BoundRules boundRules(x, y, width, height);

        switch (particle.physicsType)
        {
        case PhysicsType::Sand:
            p_sand(particle, x, y, boundRules);
            break;
        case PhysicsType::Water:
            p_water(particle, x, y, boundRules);
            break;
        }
    };

    for (int y = height - 1; y >= 0; --y)
    {
        int updateOrder = Random::genInt(0, 1);

        switch (updateOrder)
        {
        case 0:
            for (int x = width - 1; x >= 0; --x)
            {
                updateParticle(x, y);
            }
            break;
        case 1:
            for (int x = 0; x < width; ++x)
            {
                updateParticle(x, y);
            }
            break;
        }
    }
}