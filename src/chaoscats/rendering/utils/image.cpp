#include "../draw.hpp"

sf::Vector2f imageResize(const sf::Vector2u &imageSize, const sf::Vector2f &size)
{
    float aspect = imageSize.y / static_cast<float>(imageSize.x);

    bool autoHeight = size.y <= 0;

    float sizeX = size.x;
    float sizeY = autoHeight ? (size.x * aspect) : size.y;

    return sf::Vector2f(sizeX, sizeY);
}