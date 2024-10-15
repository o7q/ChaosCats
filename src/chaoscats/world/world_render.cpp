#include "world.hpp"

void World::initWorldTexture()
{
    worldPixels = new sf::Uint8[width * height * 4];
    worldTexture.create(width, height);
}

void World::render(sf::RenderWindow &renderWindow, GameState::WindowState &windowState, Camera *camera)
{
    worldTexture.update(worldPixels);

    drawTexture(
        renderWindow,
        windowState,
        worldTexture,
        sf::Vector2f(
            (width * pixelSize) / 2,
            (height * pixelSize) / 2),
        sf::Vector2f(width * pixelSize,
                     height * pixelSize),
        camera);

    for (EdgeData edge : collisionMesh)
    {
        drawSegment(renderWindow, windowState, edge.segment, camera, sf::Color(150, 150, 150, 180));
    }
}