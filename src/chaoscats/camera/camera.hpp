#pragma once

#include <cmath>

#include "../math/number.hpp"

class Camera
{
public:
    void addPosition(float x, float y)
    {
        this->x += x / zoom;
        this->y += y / zoom;
    }

    void setPosition(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void addZoom(float zoom)
    {
        this->zoom *= zoom;
    }

    void setZoom(float zoom)
    {
        this->zoom = zoom;
    }

    void setPositionGoal(float x_goal, float y_goal)
    {
        this->x_goal = x_goal;
        this->y_goal = y_goal;
    }

    float getX() const
    {
        return x;
    }

    float getY() const
    {
        return y;
    }

    float getZoom() const
    {
        return zoom;
    }

    void update(float dt)
    {
        update_position(dt);
    }

private:
    float smooth(float value)
    {
        float value_abs = std::fabs(value);

        if (value_abs < goal_smoothThreshold)
        {
            return std::fabs(value + goal_smoothThreshold * nsign(value)) / (value_abs + 1.0f);
        }

        return 1.0f;
    }

    void update_position(float dt)
    {
        float x_diff = x_goal - x;
        float y_diff = y_goal - y;

        float x_new = x;
        float y_new = y;

        float divX = smooth(x_diff);
        float divY = smooth(y_diff);

        if (x_diff > dt * goal_speed)
        {
            x_new = x + (dt * goal_speed) / divX;
        }
        if (x_diff < -dt * goal_speed)
        {
            x_new = x - (dt * goal_speed) / divX;
        }
        if (y_diff > dt * goal_speed)
        {
            y_new = y + (dt * goal_speed) / divY;
        }
        if (y_diff < -dt * goal_speed)
        {
            y_new = y - (dt * goal_speed) / divY;
        }

        setPosition(x_new, y_new);
    }

    float x = 0;
    float y = 0;

    float zoom = 1.0f;

    float x_goal = 0.0f;
    float y_goal = 0.0f;

    float zoom_goal = 1.0f;

    float goal_smoothThreshold = 300.0f;
    float goal_speed = 20.0f;
};