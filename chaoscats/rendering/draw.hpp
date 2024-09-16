#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../math/geometry/geometry_common.hpp"

enum class DrawMode
{
    Solid,
    Outline
};

template <typename T>
void drawRect(
    sf::RenderWindow &renderWindow,
    Rect<T> rectangle,
    sf::Color color = sf::Color(255, 255, 255, 255),
    DrawMode drawMode = DrawMode::Solid,
    float rotation = 0.0f)
{
    sf::RectangleShape rect;

    rect.setPosition(sf::Vector2f(rectangle.getPosition().x, rectangle.getPosition().y));
    rect.setSize(sf::Vector2f(rectangle.size.w, rectangle.size.h));
    rect.setRotation(rotation);
    switch (drawMode)
    {
    case DrawMode::Solid:
        rect.setFillColor(color);
        break;
    case DrawMode::Outline:
        rect.setOutlineColor(color);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineThickness(1);
        break;
    }

    rect.setOrigin(rectangle.size.w / 2, rectangle.size.h / 2);

    renderWindow.draw(rect);
}

template <typename T>
void drawSegment(sf::RenderWindow &renderWindow, Segment<T> segment, sf::Color color = sf::Color(255, 255, 255, 255))
{
    sf::Vertex line[2];
    line[0].position = sf::Vector2f(segment.p1.x, segment.p1.y);
    line[0].color = color;
    line[1].position = sf::Vector2f(segment.p2.x, segment.p2.y);
    line[1].color = color;

    renderWindow.draw(line, 2, sf::Lines);
}

void drawTexture(sf::RenderWindow &renderWindow, sf::Texture texture, sf::Vector2f position, sf::Vector2f size, float rotation = 0.0f);
void drawText(sf::RenderWindow &renderWindow, const std::string &textString, sf::Font &font, sf::Vector2f position, int characterSize = 24, sf::Color color = sf::Color(255, 255, 255, 255));
sf::Vector2f imageAspect(sf::Texture& texture, sf::Vector2f size);