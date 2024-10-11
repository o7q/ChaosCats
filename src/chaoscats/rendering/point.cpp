#include "draw.hpp"

void drawPoint(
    sf::RenderWindow &renderWindow,
    const Vector<float>& point,
    const Camera *camera,
    const sf::Color &color,
    const float& size)
{
    std::pair<sf::Vector2f, sf::Vector2f> transformedSprite = transformSprite(sf::Vector2f(point.x, point.y), sf::Vector2f(size, size), camera, renderWindow.getSize());

    sf::CircleShape circleShape(transformedSprite.second.x);

    circleShape.setOrigin(transformedSprite.second.x / 2.0f, transformedSprite.second.x / 2.0f);
    circleShape.setPosition(transformedSprite.first.x, transformedSprite.first.y);
    circleShape.setFillColor(color);

    renderWindow.draw(circleShape);
}