#pragma once

#include <vector>

#include "../math/types/vector.hpp"
#include "../game_state/game_state.hpp"

class Camera
{
public:
    void position_move(float x, float y);
    void position_set(float x, float y);
    void position_setGoal(float x_goal, float y_goal, float goal_speed = 0.05f);
    float getX() const;
    float getY() const;

    void zoom_move(float zoom);
    void zoom_set(float zoom);
    void zoom_setGoal(float zoom_goal, float goal_speed = 0.05f);
    float getZoom() const;

    void focus_positions(std::vector<Vector<float>> &positions, GameState::WindowState &windowState);

    void jerk(float amount);
    void jerk_directional(float direction_x, float direction_y);
    void shake(float amount, float length);

    void update(float dt);
    void update_shake(float dt);

private:
    void update_position(float dt);
    void update_zoom(float dt);

    float x = 0;
    float y = 0;
    float x_goal = 0.0f;
    float y_goal = 0.0f;
    float position_goal_speed = 0.05f;
    bool position_has_goal = false;

    float zoom = 1.0f;
    float zoom_goal = 1.0f;
    float zoom_goal_speed = 0.05f;
    bool zoom_has_goal = false;

    float shake_amount = 0.0f;
    float shake_deplete_amount = 0.0f;
    float shake_length = 0.0f;
    float shake_counter = 0.0f;
    float shake_start_x = 0.0f;
    float shake_start_y = 0.0f;
    bool is_shaking = false;
};