#include "world.hpp"

void World::initWorldTexture()
{
    worldPixels = new sf::Uint8[width * height * 4];
    worldTexture.create(width, height);
}

void World::render(sf::RenderWindow &renderWindow, GameState::WindowState &windowState, Camera *camera)
{
    if (backgroundTexture != nullptr)
    {
        drawTexture(
            renderWindow,
            windowState,
            *backgroundTexture,
            sf::Vector2f(
                (width * pixelSize) / 2.0f + camera->getX() / 2.0f,
                (height * pixelSize) / 2.0f + camera->getY() / 2.0f),
            sf::Vector2f(width * pixelSize * 2.0f,
                         height * pixelSize * 2.0f),
            camera);
    }

    worldTexture.update(worldPixels);

    Rect<float> worldBounds((width * pixelSize) / 2.0f, (height * pixelSize) / 2.0f, width * pixelSize, height * pixelSize);
    drawRect(renderWindow, windowState, worldBounds, camera, sf::Color(255, 0, 0, 100), DrawMode::Outline, 0.0f, 5.0f);

    drawTexture(
        renderWindow,
        windowState,
        worldTexture,
        sf::Vector2f(
            (width * pixelSize) / 2.0f,
            (height * pixelSize) / 2.0f),
        sf::Vector2f(width * pixelSize,
                     height * pixelSize),
        camera);

    for (EdgeData edge : collisionMesh)
    {
        drawSegment(renderWindow, windowState, edge.segment, camera, sf::Color(150, 150, 150, 180));
    }
}