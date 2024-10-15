#include "../draw.hpp"

sf::Vector2f point_worldToScreen(
    const sf::Vector2f &point,
    const Camera *camera,
    const GameState::WindowState &windowState)
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

    sf::Vector2f newPosition = sf::Vector2f(
        (point.x - camera_x - windowState.RESIZE_OFFSET_X) - windowState.WIDTH / 2.0f,
        (point.y - camera_y - windowState.RESIZE_OFFSET_Y) - windowState.HEIGHT / 2.0f);
    // apply scale transformations
    newPosition = sf::Vector2f(
        newPosition.x * camera_zoom * windowState.RESIZE_SCALE * windowState.ASPECT_SCALE,
        newPosition.y * camera_zoom * windowState.RESIZE_SCALE * windowState.ASPECT_SCALE);
    // translate screen back to the center
    newPosition = sf::Vector2f(
        newPosition.x + windowState.WIDTH / 2.0f,
        newPosition.y + windowState.HEIGHT / 2.0f);

    return newPosition;
}

sf::Vector2f point_screenToWorld(
    const sf::Vector2f &point,
    const Camera *camera,
    const GameState::WindowState &windowState)
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

    sf::Vector2f newPosition = sf::Vector2f(
        point.x - windowState.WIDTH / 2.0f,
        point.y - windowState.HEIGHT / 2.0f);
    // apply scale transformations
    newPosition = sf::Vector2f(
        newPosition.x / (camera_zoom * windowState.RESIZE_SCALE * windowState.ASPECT_SCALE),
        newPosition.y / (camera_zoom * windowState.RESIZE_SCALE * windowState.ASPECT_SCALE));
    // translate screen back to the center
    newPosition = sf::Vector2f(
        newPosition.x + camera_x + windowState.RESIZE_OFFSET_X + windowState.WIDTH / 2.0f,
        newPosition.y + camera_y + windowState.RESIZE_OFFSET_Y + windowState.HEIGHT / 2.0f);

    return newPosition;
}

sf::Vector2f size_worldToScreen(
    const sf::Vector2f &size,
    const Camera *camera,
    const GameState::WindowState &windowState)
{
    float camera_zoom = 1.0f;

    if (camera != nullptr)
    {
        camera_zoom = camera->getZoom();
    }

    sf::Vector2f newSize = sf::Vector2f(
        size.x * camera_zoom * windowState.RESIZE_SCALE * windowState.ASPECT_SCALE,
        size.y * camera_zoom * windowState.RESIZE_SCALE * windowState.ASPECT_SCALE);

    return newSize;
}

std::pair<sf::Vector2f, sf::Vector2f> sprite_worldToScreen(
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera,
    const GameState::WindowState &windowState)
{
    sf::Vector2f newPosition = point_worldToScreen(position, camera, windowState);
    sf::Vector2f newScale = size_worldToScreen(size, camera, windowState);

    return std::pair<sf::Vector2f, sf::Vector2f>(newPosition, newScale);
}