#include "../world.hpp"

void World::p_sand(Particle particle, int x, int y, BoundRules boundRules)
{
    const Particle self = particle;

    if (boundRules.canDown)
    {
        if (particles.get(y + 1, x).passable())
        {
            setParticle(x, y + 1, self);
            removeParticle(x, y);
        }
    }

    if (boundRules.canDown && boundRules.canLeft)
    {
        if (particles.get(y + 1, x - 1).passable() &&
            particles.get(y, x - 1).passable() &&
            particles.get(y, x).matterType == MatterType::Solid)
        {
            setParticle(x - 1, y + 1, self);
            removeParticle(x, y);
        }
    }

    if (boundRules.canDown && boundRules.canRight)
    {
        if (particles.get(y + 1, x + 1).passable() &&
            particles.get(y, x + 1).passable() &&
            particles.get(y, x).matterType == MatterType::Solid)
        {
            setParticle(x + 1, y + 1, self);
            removeParticle(x, y);
        }
    }
}