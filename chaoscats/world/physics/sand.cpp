#include "../world.hpp"

void World::p_sand(Particle particle, int x, int y, BoundRules boundRules)
{
    const Particle self = particle;
    bool hasMoved = false;

    if (boundRules.canDown)
    {
        if (particles.get(y + 1, x).isPassable())
        {
            setParticle(x, y + 1, self);
            removeParticle(x, y); 

            hasMoved = true;
        }
    }

    if (boundRules.canDown && boundRules.canLeft)
    {
        if (particles.get(y + 1, x - 1).isPassable() &&
            particles.get(y,     x - 1).isPassable() &&
            !hasMoved)
        {
            setParticle(x - 1, y + 1, self);
            removeParticle(x, y);

            hasMoved = true;
        }
    }

    if (boundRules.canDown && boundRules.canRight)
    {
        if (particles.get(y + 1, x + 1).isPassable() &&
            particles.get(y,     x + 1).isPassable() &&
            !hasMoved)
        {
            setParticle(x + 1, y + 1, self);
            removeParticle(x, y);

            hasMoved = true;
        }
    }
}