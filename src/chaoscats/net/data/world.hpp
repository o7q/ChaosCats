#pragma once

#include <cstdint>

#include "../../world/particle.hpp"

struct ParticleData
{
    Particle particle;
    int worldX = 0;
    int worldY = 0;
};