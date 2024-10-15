#pragma once

#include <iostream>
#include <cmath>
#include <utility>

#include <SFML/Graphics.hpp>

#include "../camera/camera.hpp"
#include "../math/types/types_common.hpp"
#include "../game_state/game_state.hpp"

enum class DrawMode
{
    Solid,
    Outline
};

// utils
sf::Vector2f imageResize(const sf::Vector2u &imageSize, const sf::Vector2f &size);

sf::Vector2f point_worldToScreen(
    const sf::Vector2f &point,
    const Camera *camera,
    const GameState::WindowState &windowState);

sf::Vector2f point_screenToWorld(
    const sf::Vector2f &point,
    const Camera *camera,
    const GameState::WindowState &windowState);

sf::Vector2f size_worldToScreen(
    const sf::Vector2f &size,
    const Camera *camera,
    const GameState::WindowState &windowState);

std::pair<sf::Vector2f, sf::Vector2f> sprite_worldToScreen(
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera,
    const GameState::WindowState &windowState);
//

// sprite
void drawSprite(
    sf::RenderWindow &renderWindow,
    const GameState::WindowState &windowState,
    sf::Sprite sprite,
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera = nullptr,
    const float &rotation = 0.0f,
    const sf::Vector2i &flipVector = sf::Vector2i(1, 1));
//

// texture
void drawTexture(
    sf::RenderWindow &renderWindow,
    const GameState::WindowState &windowState,
    const sf::Texture &texture,
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera = nullptr,
    const float &rotation = 0.0f,
    const sf::Vector2i &flipVector = sf::Vector2i(1, 1));
//

// text
void drawText(
    sf::RenderWindow &renderWindow,
    const GameState::WindowState &windowState,
    const std::string &textString,
    const sf::Font &font,
    const sf::Vector2f &position,
    const float &characterSize,
    const Camera *camera = nullptr,
    const sf::Color &color = sf::Color(255, 255, 255, 255));
//

// rectangle
void drawRect(
    sf::RenderWindow &renderWindow,
    const GameState::WindowState &windowState,
    const Rect<float> &rectangle,
    const Camera *camera = nullptr,
    const sf::Color &color = sf::Color(255, 255, 255, 255),
    const DrawMode &drawMode = DrawMode::Solid,
    const float &rotation = 0.0f);
//

// point
void drawPoint(
    sf::RenderWindow &renderWindow,
    const GameState::WindowState &windowState,
    const Vector<float> &point,
    const Camera *camera = nullptr,
    const sf::Color &color = sf::Color(255, 255, 255, 255),
    const float &size = 2.0f);
//

// segment
void drawSegment(
    sf::RenderWindow &renderWindow,
    const GameState::WindowState &windowState,
    const Segment<float> &segment,
    const Camera *camera = nullptr,
    const sf::Color &color = sf::Color(255, 255, 255, 255));
//