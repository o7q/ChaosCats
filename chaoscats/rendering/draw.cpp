#include "draw.hpp"

void drawTexture(sf::RenderWindow &renderWindow, sf::Texture texture, sf::Vector2f position, sf::Vector2f size, float rotation, sf::Vector2i flipVector)
{
    sf::Sprite textureSprite;
    textureSprite.setTexture(texture);
    textureSprite.setRotation(rotation);

    sf::Vector2u textureSize = texture.getSize();

    sf::Vector2f newSize = imageAspect(texture, size);

    textureSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
    textureSprite.setPosition(position.x, position.y);
    textureSprite.setScale(newSize.x / textureSize.x * flipVector.x, newSize.y / textureSize.y * flipVector.y);

    renderWindow.draw(textureSprite);
}

void drawText(sf::RenderWindow &renderWindow, const std::string &textString, sf::Font &font, sf::Vector2f position, int characterSize, sf::Color color)
{
    sf::Text text;
    
    text.setString(textString);
    text.setFont(font);
    text.setPosition(position);
    text.setCharacterSize(characterSize);
    text.setFillColor(color);

    renderWindow.draw(text);
}

sf::Vector2f imageAspect(sf::Texture &texture, sf::Vector2f size)
{
    sf::Vector2u textureSize = texture.getSize();
    float aspect = textureSize.y / static_cast<float>(textureSize.x);

    bool autoHeight = size.y <= 0;

    float sizeX = size.x;
    float sizeY = autoHeight ? (size.x * aspect) : size.y;

    return sf::Vector2f(sizeX, sizeY);
}