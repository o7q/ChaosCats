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

    auto mixColor = [this](sf::Color color1, sf::Color color2)
    {
        return sf::Color(color1.r / 2 + color2.r / 2,
                         color1.g / 2 + color2.g / 2,
                         color1.b / 2 + color2.b / 2,
                         color1.a / 2 + color2.a / 2);
    };

    if (boundRules.canDown)
    {
        if (particles.get(y + 1, x).isLiquid() &&
            !hasMoved)
        {
            Particle tradeParticle = getParticle(x, y + 1);

            sf::Color newColor = mixColor(tradeParticle.color, self.color);

            Particle newParticle = self;
            newParticle.color = newColor;
            tradeParticle.color = newColor;

            setParticle(x, y, newParticle);
            setParticle(x, y + 1, tradeParticle);
        }
    }

    if (boundRules.canLeft)
    {
        if (particles.get(y, x - 1).isLiquid() &&
            !hasMoved)
        {
            Particle tradeParticle = getParticle(x - 1, y);

            sf::Color newColor = mixColor(tradeParticle.color, self.color);

            Particle newParticle = self;
            newParticle.color = newColor;
            tradeParticle.color = newColor;

            setParticle(x, y, newParticle);
            setParticle(x - 1, y, tradeParticle);
        }
    }

    if (boundRules.canRight)
    {
        if (particles.get(y, x + 1).isLiquid() &&
            !hasMoved)
        {
            Particle tradeParticle = getParticle(x + 1, y);

            sf::Color newColor = mixColor(tradeParticle.color, self.color);

            Particle newParticle = self;
            newParticle.color = newColor;
            tradeParticle.color = newColor;

            setParticle(x, y, newParticle);
            setParticle(x + 1, y, tradeParticle);
        }
    }
}