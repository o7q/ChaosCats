#pragma once

#include <iostream>
#include <cmath>
#include <utility>

#include <SFML/Graphics.hpp>

#include "../camera/camera.hpp"
#include "../math/geometry/geometry_common.hpp"

enum class DrawMode
{
    Solid,
    Outline
};

// utils
sf::Vector2f imageAspect(const sf::Vector2u &imageSize, sf::Vector2f size);

std::pair<sf::Vector2f, sf::Vector2f> transformSprite(
    const sf::Vector2u &windowSize,
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera = nullptr);
//

// texture
void drawTexture(
    sf::RenderWindow &renderWindow,
    const sf::Texture &texture,
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera = nullptr,
    const float &rotation = 0.0f,
    const sf::Vector2i &flipVector = sf::Vector2i(1, 1));
//

// rectangle
void drawRect(
    sf::RenderWindow &renderWindow,
    const Rect<float> &rectangle,
    const Camera *camera = nullptr,
    const sf::Color &color = sf::Color(255, 255, 255, 255),
    const DrawMode &drawMode = DrawMode::Solid,
    const float &rotation = 0.0f);
//