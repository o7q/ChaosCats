#include "world.hpp"

net::Message<GameMessages> World::drawParticles(int loc_x, int loc_y, Particle particle, int radius = 5, DrawShape drawShape = DrawShape::Circle)
{
    std::vector<ParticleData> updatedParticles;

    switch (drawShape)
    {
    case DrawShape::Square:
        for (int y = 0; y < radius; ++y)
        {
            for (int x = 0; x < radius; ++x)
            {
                int x_clamped = std::clamp(loc_x + x - radius / 2, 0, width - 1);
                int y_clamped = std::clamp(loc_y + y - radius / 2, 0, height - 1);

                setParticle(x_clamped, y_clamped, particle);

                ParticleData particleData{particle, x_clamped, y_clamped};
                updatedParticles.push_back(particleData);
            }
        }
        break;
    case DrawShape::Circle:
        for (int y = loc_y - radius; y <= loc_y + radius; ++y)
        {
            for (int x = loc_x - radius; x <= loc_x + radius; ++x)
            {
                if ((x - loc_x) * (x - loc_x) + (y - loc_y) * (y - loc_y) <= radius * radius)
                {
                    int x_clamped = std::clamp(x, 0, width - 1);
                    int y_clamped = std::clamp(y, 0, height - 1);

                    setParticle(x_clamped, y_clamped, particle);

                    ParticleData particleData{particle, x_clamped, y_clamped};
                    updatedParticles.push_back(particleData);
                }
            }
        }
        break;
    }

    net::Message<GameMessages> modifiedWorldMessage;

    for (size_t i = 0; i < updatedParticles.size(); ++i)
    {
        modifiedWorldMessage << updatedParticles[i];
    }

    modifiedWorldMessage << updatedParticles.size();

    return modifiedWorldMessage;
}