#include "draw.hpp"

void drawRectangle(sf::RenderWindow &renderWindow, jmath::geo::Rectangle<float> rectangle, sf::Color color, float rotation)
{
    sf::RectangleShape rect;

    rect.setPosition(sf::Vector2f(rectangle.topLeft.x + rectangle.half_size.w, rectangle.topLeft.y + rectangle.half_size.h));
    rect.setSize(sf::Vector2f(rectangle.size.w, rectangle.size.h));
    rect.setRotation(rotation);
    rect.setFillColor(color);

    rect.setOrigin(rectangle.half_size.w, rectangle.half_size.h);

    renderWindow.draw(rect);
}

void drawSegment(sf::RenderWindow &renderWindow, jmath::geo::Segment<float> segment, sf::Color color)
{
    sf::Vertex line[2];
    line[0].position = sf::Vector2f(segment.x, segment.y);
    line[0].color = color;
    line[1].position = sf::Vector2f(segment.x2, segment.y2);
    line[1].color = color;

    renderWindow.draw(line, 2, sf::Lines);
}

void drawTexture(sf::RenderWindow &renderWindow, sf::Texture texture, jmath::geo::Rectangle<float> rectangle, float rotation)
{
    sf::Sprite textureSprite;
    textureSprite.setTexture(texture);
    textureSprite.setPosition(sf::Vector2f(rectangle.topLeft.x + rectangle.half_size.w, rectangle.topLeft.y + rectangle.half_size.h));
    textureSprite.setRotation(rotation);

    sf::Vector2u textureSize = texture.getSize();

    float aspect = textureSize.y / static_cast<float>(textureSize.x);

    float sizeX = rectangle.size.w;
    float sizeY = (rectangle.size.h <= 0) ? (rectangle.size.w * aspect) : rectangle.size.h;

    textureSprite.setScale(sizeX / textureSize.x, sizeY / textureSize.y);
    textureSprite.setOrigin(textureSize.x / 2, (rectangle.size.h <= 0) ? (textureSize.y * aspect) / 2 : (textureSize.y / 2));

    renderWindow.draw(textureSprite);
}