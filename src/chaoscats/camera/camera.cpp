#include <cmath>

#include "camera.hpp"
#include "../math/number.hpp"

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
    this->goal_speed = goal_speed;
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
    this->goal_speed = goal_speed;
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

    x = lerp(x, x_goal, goal_speed * dt);
    y = lerp(y, y_goal, goal_speed * dt);

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

    zoom = lerp(zoom, zoom_goal, goal_speed * dt);

    if (std::fabs(zoom - zoom_goal) < 0.01f)
    {
        zoom = zoom_goal;
    }
}