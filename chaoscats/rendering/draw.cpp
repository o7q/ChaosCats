#include "draw.hpp"

void drawTexture(sf::RenderWindow &renderWindow, sf::Texture texture, sf::Vector2f position, sf::Vector2f size, float rotation)
{
    sf::Sprite textureSprite;
    textureSprite.setTexture(texture);
    textureSprite.setRotation(rotation);

    sf::Vector2u textureSize = texture.getSize();

    float aspect = textureSize.y / static_cast<float>(textureSize.x);

    bool autoHeight = size.y <= 0;

    float sizeX = size.x;
    float sizeY = autoHeight ? (size.x * aspect) : size.y;

    textureSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
    textureSprite.setPosition(position.x + sizeX / 2, position.y + sizeY / 2);
    textureSprite.setScale(sizeX / textureSize.x, sizeY / textureSize.y);

    renderWindow.draw(textureSprite);
}