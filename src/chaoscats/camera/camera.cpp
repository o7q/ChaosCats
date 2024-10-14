#include <cmath>
#include <limits>

#include <SFML/Graphics.hpp>

#include "camera.hpp"
#include "../math/number.hpp"
#include "../math/types/vector.hpp"
#include "../rendering/draw.hpp"

void Camera::position_move(float x, float y)
{
    this->x += x / zoom;
    this->y += y / zoom;
    this->position_has_goal = false;
}

void Camera::position_set(float x, float y)
{
    this->x = x;
    this->y = y;
    this->position_has_goal = false;
}

void Camera::position_setGoal(float x_goal, float y_goal, float goal_speed)
{
    this->x_goal = x_goal;
    this->y_goal = y_goal;
    this->position_goal_speed = goal_speed;
    this->position_has_goal = true;
}

float Camera::getX() const
{
    return x;
}

float Camera::getY() const
{
    return y;
}

void Camera::zoom_move(float zoom)
{
    this->zoom *= zoom;
    this->zoom_has_goal = false;
}

void Camera::zoom_set(float zoom)
{
    this->zoom = zoom;
    this->zoom_has_goal = false;
}

void Camera::zoom_setGoal(float zoom_goal, float goal_speed)
{
    this->zoom_goal = zoom_goal;
    this->zoom_goal_speed = goal_speed;
    this->zoom_has_goal = true;
}

float Camera::getZoom() const
{
    return zoom;
}

void Camera::update(float dt)
{
    update_position(dt);
    update_zoom(dt);
}

void Camera::update_position(float dt)
{
    if (!position_has_goal)
    {
        return;
    }

    x = lerp(x, x_goal, position_goal_speed * dt);
    y = lerp(y, y_goal, position_goal_speed * dt);

    if (std::fabs(x - x_goal) < 1.0f)
    {
        x = x_goal;
    }

    if (std::fabs(y - y_goal) < 1.0f)
    {
        y = y_goal;
    }
}

void Camera::update_zoom(float dt)
{
    if (!zoom_has_goal)
    {
        return;
    }

    zoom = lerp(zoom, zoom_goal, zoom_goal_speed * dt);

    if (std::fabs(zoom - zoom_goal) < 0.01f)
    {
        zoom = zoom_goal;
    }
}

void Camera::focus_positions(std::vector<Vector<float>> &positions, GameInfo::WindowInfo &windowInfo)
{
    constexpr float FLOAT_MIN = std::numeric_limits<float>::min();
    constexpr float FLOAT_MAX = std::numeric_limits<float>::max();

    float min_x = FLOAT_MAX;
    float min_y = FLOAT_MAX;
    float max_x = FLOAT_MIN;
    float max_y = FLOAT_MIN;

    for (Vector<float> position : positions)
    {
        min_x = std::min(position.x, min_x);
        min_y = std::min(position.y, min_y);
        max_x = std::max(position.x, max_x);
        max_y = std::max(position.y, max_y);
    }

    float padding = 200.0f;

    // calcuate midpoint
    float x_mid = (max_x - padding / 2.0f + min_x + padding / 2.0f) / 2.0f - (windowInfo.WIDTH - padding) / 2.0f;
    float y_mid = (max_y - padding / 2.0f + min_y + padding / 2.0f) / 2.0f - (windowInfo.HEIGHT - padding) / 2.0f;

    float x_scale = std::min(1.0f, (windowInfo.WIDTH - padding) / std::fabs(max_x - padding / 2.0f - min_x + padding / 2.0f));
    float y_scale = std::min(1.0f, (windowInfo.HEIGHT - padding) / std::fabs(max_y - padding / 2.0f - min_y + padding / 2.0f));

    float scale = std::min(x_scale, y_scale);

    position_setGoal(x_mid, y_mid, 0.15f);
    zoom_setGoal(scale / windowInfo.RESIZE_SCALE, 0.15f);
}