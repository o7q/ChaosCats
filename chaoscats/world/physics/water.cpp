#include "../world.hpp"

void World::p_water(Particle particle, int x, int y, BoundRules boundRules)
{
    const Particle self = particle;

    if (boundRules.canDown)
    {
        if (particles.get(y + 1, x).air())
        {
            setParticle(x, y + 1, self);
            removeParticle(x, y); 
        }
    }

    if (boundRules.canLeft)
    {
        if (particles.get(y, x - 1).air() &&
            particles.get(y, x        ).matterType == MatterType::Liquid)
        {
            setParticle(x - 1, y, self);
            removeParticle(x, y);
        }
    }

    if (boundRules.canRight)
    {
        if (particles.get(y, x + 1).air() &&
            particles.get(y,     x    ).matterType == MatterType::Liquid)
        {
            setParticle(x + 1, y, self);
            removeParticle(x, y);
        }
    }
}