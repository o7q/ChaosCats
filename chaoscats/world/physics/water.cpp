#include "../world.hpp"

void World::p_water(Particle particle, int x, int y, BoundRules boundRules)
{
    const Particle self = particle;
    bool hasMoved = false;

    if (boundRules.canDown)
    {
        if (particles.get(y + 1, x).isAir())
        {
            setParticle(x, y + 1, self);
            removeParticle(x, y); 

            hasMoved = true;
        }
    }

    if (boundRules.canLeft)
    {
        if (particles.get(y, x - 1).isAir() &&
            !hasMoved)
        {
            setParticle(x - 1, y, self);
            removeParticle(x, y);

            hasMoved = true;
        }
    }

    if (boundRules.canRight)
    {
        if (particles.get(y, x + 1).isAir() &&
            !hasMoved)
        {
            setParticle(x + 1, y, self);
            removeParticle(x, y);

            hasMoved = true;
        }
    }
}