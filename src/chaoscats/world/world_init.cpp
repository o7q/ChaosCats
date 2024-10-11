#include "world.hpp"

#include "../utils/logger.hpp"

void World::initWorldTexture()
{
    worldPixels = new sf::Uint8[width * height * 4];
    worldTexture.create(width, height);
}