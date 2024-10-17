#include "camera.hpp"

#include <cmath>
#include <limits>

#include <SFML/Graphics.hpp>

#include "../math/random.hpp"

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

void Camera::focus_positions(std::vector<Vector<float>> &positions, GameState::WindowState &windowState)
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
    float x_mid = (max_x - padding / 2.0f + min_x + padding / 2.0f) / 2.0f - (windowState.WIDTH - padding) / 2.0f;
    float y_mid = (max_y - padding / 2.0f + min_y + padding / 2.0f) / 2.0f - (windowState.HEIGHT - padding) / 2.0f;

    float x_scale = std::min(1.0f, (windowState.WIDTH - padding) / std::fabs(max_x - padding / 2.0f - min_x + padding / 2.0f));
    float y_scale = std::min(1.0f, (windowState.HEIGHT - padding) / std::fabs(max_y - padding / 2.0f - min_y + padding / 2.0f));

    float scale = std::min(x_scale, y_scale);

    position_setGoal(x_mid, y_mid, 0.15f);
    zoom_setGoal(scale / windowState.RESIZE_SCALE, 0.15f);
}

void Camera::jerk(float amount)
{
    float jerk_x = x + Random::genFloat(-amount / 2.0f, amount / 2.0f);
    float jerk_y = y + Random::genFloat(-amount / 2.0f, amount / 2.0f);

    position_set(jerk_x, jerk_y);
}

void Camera::jerk_directional(float direction_x, float direction_y)
{
    float jerk_x = x + direction_x;
    float jerk_y = y + direction_y;

    position_set(jerk_x, jerk_y);
}

void Camera::shake(float amount, float length)
{
    this->shake_amount = amount;
    this->shake_length = length;

    shake_deplete_amount = amount / length;

    shake_start_x = x;
    shake_start_y = y;

    shake_counter = 0.0f;

    is_shaking = true;
}