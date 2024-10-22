#include "draw.hpp"

void drawText(
    sf::RenderWindow &renderWindow,
    const GameState::WindowState &windowState,
    const std::string &textString,
    const sf::Font &font,
    const sf::Vector2f &position,
    const float &characterSize,
    const Camera *camera,
    const sf::Color &color)
{
    sf::Text text;
    text.setString(textString);
    text.setFont(font);
    text.setCharacterSize(characterSize);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

    std::pair<sf::Vector2f, sf::Vector2f> transformedSprite = sprite_worldToScreen(position, sf::Vector2f(textBounds.width, textBounds.height), camera, windowState);

    text.setPosition(transformedSprite.first.x, transformedSprite.first.y);
    // text.setCharacterSize(transformedSprite.second.y);
    text.setFillColor(color);

    renderWindow.draw(text);
}