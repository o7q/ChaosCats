#include "draw.hpp"

void drawTexture(sf::RenderWindow &renderWindow, sf::Texture texture, sf::Vector2f position, sf::Vector2f size, float rotation)
{
    sf::Sprite textureSprite;
    textureSprite.setTexture(texture);
    textureSprite.setRotation(rotation);

    sf::Vector2u textureSize = texture.getSize();

    sf::Vector2f newSize = imageAspect(texture, size);

    textureSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
    textureSprite.setPosition(position.x + newSize.x / 2, position.y + newSize.y / 2);
    textureSprite.setScale(newSize.x / textureSize.x, newSize.y / textureSize.y);

    renderWindow.draw(textureSprite);
}

sf::Vector2f imageAspect(sf::Texture& texture, sf::Vector2f size)
{
    sf::Vector2u textureSize = texture.getSize();
    float aspect = textureSize.y / static_cast<float>(textureSize.x);

    bool autoHeight = size.y <= 0;

    float sizeX = size.x;
    float sizeY = autoHeight ? (size.x * aspect) : size.y;

    return sf::Vector2f(sizeX, sizeY);
}