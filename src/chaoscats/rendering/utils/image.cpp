#include "../draw.hpp"

sf::Vector2f imageAspect(const sf::Vector2u &imageSize, sf::Vector2f size)
{
    float aspect = imageSize.y / static_cast<float>(imageSize.x);

    bool autoHeight = size.y <= 0;

    float sizeX = size.x;
    float sizeY = autoHeight ? (size.x * aspect) : size.y;

    return sf::Vector2f(sizeX, sizeY);
}

std::pair<sf::Vector2f, sf::Vector2f> transformSprite(
    const sf::Vector2u &windowSize,
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera)
{
    constexpr float baseAspect = 720.0f / 1280.0f;
    const float screenAspect = windowSize.y / static_cast<float>(windowSize.x);

    const float aspect = baseAspect / screenAspect;

    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraZoom = 1.0f;

    if (camera != nullptr)
    {
        cameraX = camera->getX();
        cameraY = camera->getY();
        cameraZoom = camera->getZoom();
    }

    sf::Vector2f scaleOffset;

    if (aspect < 1.0f)
    {
        scaleOffset = sf::Vector2f(1.0f, aspect);
    }
    else
    {
        scaleOffset = sf::Vector2f(1.0f / aspect, 1.0f);
    }

    // translate the sprite, then translate the screen to the top left
    sf::Vector2f newPosition = sf::Vector2f(
        (position.x - cameraX) - windowSize.x / 2.0f,
        (position.y - cameraY) - windowSize.y / 2.0f);
    // apply scale transformations
    newPosition = sf::Vector2f(
        newPosition.x * scaleOffset.x * cameraZoom,
        newPosition.y * scaleOffset.y * cameraZoom);
    // translate screen back to the center
    newPosition = sf::Vector2f(
        newPosition.x + windowSize.x / 2.0f,
        newPosition.y + windowSize.y / 2.0f);

    // calculate the scale, no screen translations are required because sfml scales from the center by default
    sf::Vector2f newScale = sf::Vector2f(
        size.x * scaleOffset.x * cameraZoom,
        size.y * scaleOffset.y * cameraZoom);

    return std::pair<sf::Vector2f, sf::Vector2f>(newPosition, newScale);
}