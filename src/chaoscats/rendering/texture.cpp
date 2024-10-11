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

    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2f newSize = imageAspect(textureSize, size);

    std::pair<sf::Vector2f, sf::Vector2f> transformedSprite = transformSprite(renderWindow.getSize(), position, newSize, camera);

    textureSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
    textureSprite.setPosition(transformedSprite.first.x, transformedSprite.first.y);
    textureSprite.setScale(transformedSprite.second.x / textureSize.x * flipVector.x, transformedSprite.second.y / textureSize.y * flipVector.y);
    textureSprite.setRotation(rotation);

    renderWindow.draw(textureSprite);
}