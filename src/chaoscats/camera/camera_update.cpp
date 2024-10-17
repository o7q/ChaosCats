#include "camera.hpp"

#include <cmath>

#include "../math/random.hpp"
#include "../math/number.hpp"

void Camera::update(float dt)
{
    update_position(dt);
    update_zoom(dt);
    update_shake(dt);
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

void Camera::update_shake(float dt)
{
    if (!is_shaking)
    {
        return;
    }

    if (shake_counter > shake_length)
    {
        is_shaking = false;

        position_setGoal(shake_start_x, shake_start_y);
    }

    float random_x = x + Random::genFloat(-shake_amount / 2.0, shake_amount / 2.0f);
    float random_y = y + Random::genFloat(-shake_amount / 2.0, shake_amount / 2.0f);

    position_set(random_x, random_y);

    shake_counter += dt;
    shake_amount -= shake_deplete_amount * dt;
}