#include "draw.hpp"

void drawText(
    sf::RenderWindow &renderWindow,
    const std::string &textString,
    const sf::Font &font,
    const sf::Vector2f& position,
    const float& characterSize,
    const Camera* camera,
    const sf::Color &color)
{
    sf::Text text;

    std::pair<sf::Vector2f, sf::Vector2f> transformedSprite = transformSprite(position, sf::Vector2f(characterSize * textString.size(), characterSize), camera, renderWindow.getSize());

    text.setString(textString);
    text.setFont(font);
    text.setPosition(transformedSprite.first.x, transformedSprite.first.y);
    text.setCharacterSize(transformedSprite.second.y);
    text.setFillColor(color);

    renderWindow.draw(text);
}