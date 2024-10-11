#include "draw.hpp"

void drawTexture(
    sf::RenderWindow &renderWindow,
    const sf::Texture &texture,
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera,
    const float &rotation,
    const sf::Vector2i &flipVector)
{
    sf::Sprite textureSprite;
    textureSprite.setTexture(texture);

    drawSprite(renderWindow, textureSprite, position, size, camera, rotation, flipVector);
}

void drawSprite(
    sf::RenderWindow &renderWindow,
    sf::Sprite sprite,
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera,
    const float &rotation,
    const sf::Vector2i &flipVector)
{
    sf::Vector2u spriteSize = sprite.getTexture()->getSize();
    sf::Vector2f newSize = imageResize(spriteSize, size);

    std::pair<sf::Vector2f, sf::Vector2f> transformedSprite = transformSprite(position, newSize, camera, renderWindow.getSize());

    sprite.setOrigin(spriteSize.x / 2, spriteSize.y / 2);
    sprite.setPosition(transformedSprite.first.x, transformedSprite.first.y);
    sprite.setScale(transformedSprite.second.x / spriteSize.x * flipVector.x, transformedSprite.second.y / spriteSize.y * flipVector.y);
    sprite.setRotation(rotation);

    renderWindow.draw(sprite);
}