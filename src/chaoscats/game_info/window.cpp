#include "game_info.hpp"

#include <SFML/Graphics.hpp>

void GameInfo::WindowInfo::refresh(sf::RenderWindow &renderWindow)
{
    sf::Vector2u windowSize = renderWindow.getSize();

    WIDTH = windowSize.x;
    HEIGHT = windowSize.y;
    ASPECT = HEIGHT / static_cast<float>(WIDTH);

    RESIZE_SCALE = calculateResizeScale(windowSize);
    ASPECT_SCALE = calculateAspectScale(windowSize);
    std::pair<float, float> resizeOffset = calculateResizeOffset(windowSize);
    RESIZE_OFFSET_X = resizeOffset.first;
    RESIZE_OFFSET_Y = resizeOffset.second;
}

float GameInfo::WindowInfo::calculateResizeScale(const sf::Vector2u &windowSize)
{
    constexpr float base_window_x = 1280.0f;
    constexpr float base_window_y = 720.0f;

    const float resize_ratio_x = windowSize.x / base_window_x;
    const float resize_ratio_y = windowSize.y / base_window_y;
    const float resize_scale = std::max(resize_ratio_x, resize_ratio_y);

    return resize_scale;
}

float GameInfo::WindowInfo::calculateAspectScale(const sf::Vector2u &windowSize)
{
    constexpr float base_window_x = 1280.0f;
    constexpr float base_window_y = 720.0f;
    constexpr float baseAspect = base_window_y / base_window_x;

    const float screenAspect = windowSize.y / static_cast<float>(windowSize.x);

    const float aspect = baseAspect / screenAspect;

    float aspect_scale = 1.0f;

    if (aspect < 1.0f)
    {
        aspect_scale = aspect;
    }
    else
    {
        aspect_scale = 1.0f / aspect;
    }

    return aspect_scale;
}

std::pair<float, float> GameInfo::WindowInfo::calculateResizeOffset(const sf::Vector2u &windowSize)
{
    constexpr float base_window_x = 1280.0f;
    constexpr float base_window_y = 720.0f;

    const float x_offset = (base_window_x - windowSize.x) / 2.0f;
    const float y_offset = (base_window_y - windowSize.y) / 2.0f;

    return std::pair<float, float>(x_offset, y_offset);
}