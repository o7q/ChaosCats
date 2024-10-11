#include "../draw.hpp"

sf::Vector2f imageResize(const sf::Vector2u &imageSize, const sf::Vector2f &size)
{
    float aspect = imageSize.y / static_cast<float>(imageSize.x);

    bool autoHeight = size.y <= 0;

    float sizeX = size.x;
    float sizeY = autoHeight ? (size.x * aspect) : size.y;

    return sf::Vector2f(sizeX, sizeY);
}

float calculateResizeScale(const sf::Vector2u &windowSize)
{
    constexpr float base_window_x = 1280.0f;
    constexpr float base_window_y = 720.0f;

    const float resize_ratio_x = windowSize.x / base_window_x;
    const float resize_ratio_y = windowSize.y / base_window_y;
    const float resize_scale = std::max(resize_ratio_x, resize_ratio_y);

    return resize_scale;
}

float calculateAspectScale(const sf::Vector2u &windowSize)
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