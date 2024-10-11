#include "draw.hpp"

void drawRect(
    sf::RenderWindow &renderWindow,
    const Rect<float> &rectangle,
    const Camera *camera,
    const sf::Color &color,
    const DrawMode &drawMode,
    const float &rotation)
{
    sf::RectangleShape rectShape;

    switch (drawMode)
    {
    case DrawMode::Solid:
        rectShape.setFillColor(color);
        break;
    case DrawMode::Outline:
        rectShape.setOutlineColor(color);
        rectShape.setFillColor(sf::Color::Transparent);
        rectShape.setOutlineThickness(1);
        break;
    }

    sf::Vector2f position(rectangle.getPosition().x, rectangle.getPosition().y);
    sf::Vector2f size(rectangle.size.x, rectangle.size.y);

    std::pair<sf::Vector2f, sf::Vector2f> transformedSprite = transformSprite(position, size, camera, renderWindow.getSize());

    rectShape.setOrigin(transformedSprite.second.x / 2.0f, transformedSprite.second.x / 2.0f);
    rectShape.setPosition(transformedSprite.first.x, transformedSprite.first.y);
    rectShape.setSize(sf::Vector2f(transformedSprite.second.x, transformedSprite.second.y));
    rectShape.setRotation(rotation);

    renderWindow.draw(rectShape);
}