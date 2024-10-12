#include "draw.hpp"

void drawPoint(
    sf::RenderWindow &renderWindow,
    const GameInfo::WindowInfo &windowInfo,
    const Vector<float> &point,
    const Camera *camera,
    const sf::Color &color,
    const float &size)
{
    std::pair<sf::Vector2f, sf::Vector2f> transformedSprite = sprite_worldToScreen(sf::Vector2f(point.x - size / 2.0f, point.y - size / 2.0f), sf::Vector2f(size, size), camera, windowInfo);

    sf::CircleShape circleShape(transformedSprite.second.x);

    circleShape.setOrigin(transformedSprite.second.x / 2.0f, transformedSprite.second.x / 2.0f);
    circleShape.setPosition(transformedSprite.first.x, transformedSprite.first.y);
    circleShape.setFillColor(color);

    renderWindow.draw(circleShape);
}