#include "world.hpp"

void World::initWorldTexture()
{
    worldPixels = new sf::Uint8[width * height * 4];
    worldTexture.create(width, height);
}

void World::render(sf::RenderWindow &renderWindow, Camera *camera)
{
    worldTexture.update(worldPixels);

    drawTexture(renderWindow, worldTexture, sf::Vector2f(1280 / 2.0f, 720 / 2.0f), sf::Vector2f(width * pixelSize, -1), camera);

    for (EdgeData edge : collisionMesh)
    {
        drawSegment(renderWindow, edge.segment, camera, sf::Color(150, 150, 150, 180));
    }
}