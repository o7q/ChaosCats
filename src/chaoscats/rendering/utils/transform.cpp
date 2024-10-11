#include "../draw.hpp"

sf::Vector2f transformPoint(
    const sf::Vector2f &point,
    const Camera *camera,
    const sf::Vector2u &windowSize,
    const float &resize_scale,
    const float &aspect_scale)
{
    constexpr float base_window_x = 1280.0f;
    constexpr float base_window_y = 720.0f;

    const float x_offset = (base_window_x - windowSize.x) / 2.0f;
    const float y_offset = (base_window_y - windowSize.y) / 2.0f;

    float camera_x = 0.0f;
    float camera_y = 0.0f;
    float camera_zoom = 1.0f;

    if (camera != nullptr)
    {
        camera_x = camera->getX();
        camera_y = camera->getY();
        camera_zoom = camera->getZoom();
    }

    sf::Vector2f newPosition = sf::Vector2f(
        (point.x - camera_x - x_offset) - windowSize.x / 2.0f,
        (point.y - camera_y - y_offset) - windowSize.y / 2.0f);
    // apply scale transformations
    newPosition = sf::Vector2f(
        newPosition.x * camera_zoom * resize_scale * aspect_scale,
        newPosition.y * camera_zoom * resize_scale * aspect_scale);
    // translate screen back to the center
    newPosition = sf::Vector2f(
        newPosition.x + windowSize.x / 2.0f,
        newPosition.y + windowSize.y / 2.0f);

    return newPosition;
}

sf::Vector2f transformSize(
    const sf::Vector2f &size,
    const Camera *camera,
    const float &resize_scale,
    const float &aspect_scale)
{
    float camera_x = 0.0f;
    float camera_y = 0.0f;
    float camera_zoom = 1.0f;

    if (camera != nullptr)
    {
        camera_x = camera->getX();
        camera_y = camera->getY();
        camera_zoom = camera->getZoom();
    }

    sf::Vector2f newSize = sf::Vector2f(
        size.x * camera_zoom * resize_scale * aspect_scale,
        size.y * camera_zoom * resize_scale * aspect_scale);

    return newSize;
}

std::pair<sf::Vector2f, sf::Vector2f> transformSprite(
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera,
    const sf::Vector2u &windowSize)
{
    float resize_scale = calculateResizeScale(windowSize);
    float aspect_scale = calculateAspectScale(windowSize);

    float camera_x = 0.0f;
    float camera_y = 0.0f;
    float camera_zoom = 1.0f;

    if (camera != nullptr)
    {
        camera_x = camera->getX();
        camera_y = camera->getY();
        camera_zoom = camera->getZoom();
    }

    sf::Vector2f newPosition = transformPoint(position, camera, windowSize, resize_scale, aspect_scale);
    sf::Vector2f newScale = transformSize(size, camera, resize_scale, aspect_scale);

    return std::pair<sf::Vector2f, sf::Vector2f>(newPosition, newScale);
}