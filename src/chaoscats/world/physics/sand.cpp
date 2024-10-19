#include "../world.hpp"
#include "../../math/random.hpp"

void World::p_sand(Particle particle, int x, int y, BoundRules boundRules)
{
    const Particle self = particle;
    bool hasMoved = false;

    int randomMoveChance = Random::genInt(0, 20);

    bool canMoveThroughLiquid = randomMoveChance == 0;

    if (boundRules.canDown)
    {
        if ((particles.get(y + 1, x).isAir() ||
             (particles.get(y + 1, x).isPassable() && canMoveThroughLiquid)) &&
            !hasMoved)
        {
            setParticle(x, y + 1, self);
            removeParticle(x, y);

            hasMoved = true;
        }
    }

    if (boundRules.canDown && boundRules.canLeft)
    {

        if ((particles.get(y + 1, x - 1).isAir() || (particles.get(y + 1, x - 1).isPassable() && canMoveThroughLiquid)) &&
            (particles.get(y, x - 1).isAir() || (particles.get(y, x - 1).isPassable() && canMoveThroughLiquid)) &&
            !hasMoved)
        {
            setParticle(x - 1, y + 1, self);
            removeParticle(x, y);

            hasMoved = true;
        }
    }

    if (boundRules.canDown && boundRules.canRight)
    {
        if ((particles.get(y + 1, x + 1).isAir() || (particles.get(y + 1, x + 1).isPassable() && canMoveThroughLiquid)) &&
            (particles.get(y, x + 1).isAir() || (particles.get(y, x + 1).isPassable() && canMoveThroughLiquid)) &&
            !hasMoved)
        {
            setParticle(x + 1, y + 1, self);
            removeParticle(x, y);

            hasMoved = true;
        }
    }
}