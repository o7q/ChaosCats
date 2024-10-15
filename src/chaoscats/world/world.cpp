#include "world.hpp"

World::World(Map &map, int pixelSize, bool serverWorld) : particles(map.height, map.width)
{
    this->width = map.width;
    this->height = map.height;
    this->pixelSize = pixelSize;
    this->serverWorld = serverWorld;

    initWorldTexture();

    createWorldFromMap(map);
}

World::World(int width, int height, int pixelSize, bool serverWorld) : particles(height, width)
{
    this->width = width;
    this->height = height;
    this->pixelSize = pixelSize;
    this->serverWorld = serverWorld;

    initWorldTexture();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            removeParticle(x, y);
        }
    }
}

World::~World()
{
    if (worldPixels != nullptr)
    {
        delete[] worldPixels;
        worldPixels = nullptr;
    }
}