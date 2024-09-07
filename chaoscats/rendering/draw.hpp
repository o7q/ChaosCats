#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../math/geometry/geometry_common.hpp"

void drawRectangle(
    sf::RenderWindow &renderWindow,
    jmath::geo::Rectangle<float> rectangle,
    sf::Color color = sf::Color(255, 255, 255, 255),
    float rotation = 0.0f);
void drawSegment(
    sf::RenderWindow &renderWindow,
    jmath::geo::Segment<float> segment,
    sf::Color color = sf::Color(255, 255, 255, 255));
void drawTexture(
    sf::RenderWindow &renderWindow,
    sf::Texture texture,
    jmath::geo::Rectangle<float> rectangle,
    float rotation = 0.0f);