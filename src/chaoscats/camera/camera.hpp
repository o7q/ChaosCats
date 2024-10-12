#pragma once

#include <vector>
#include "../math/geometry/vector.hpp"
#include "../game_info/game_info.hpp"

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

    void update(float dt);

    void focus_positions(std::vector<Vector<float>> positions, GameInfo::WindowInfo &windowInfo);

private:
    void update_position(float dt);
    void update_zoom(float dt);

    float x = 0;
    float y = 0;
    float x_goal = 0.0f;
    float y_goal = 0.0f;
    bool position_has_goal = false;

    float zoom = 1.0f;
    float zoom_goal = 1.0f;
    bool zoom_has_goal = false;

    float goal_speed = 0.05f;
};