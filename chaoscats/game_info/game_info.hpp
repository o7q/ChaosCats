#pragma once

#include <chrono>

#include <SFML/Graphics.hpp>

struct GameInfo
{
private:
    struct TimeInfo
    {
        int FRAMERATE = 0;
        float DELTA_TIME = 0;

        void refresh(clock_t frame_start, clock_t frame_end);
    };

    struct WindowInfo
    {
        float ASPECT = 1.0f;
        unsigned int WIDTH = 1;
        unsigned int HEIGHT = 1;

        bool IS_FOCUSED = true;

        void refresh(sf::RenderWindow &renderWindow);
    };

    struct InputInfo
    {
        bool MOUSE_LMB_CLICKED = false;
        bool MOUSE_LMB_DOWN = false;
        sf::Vector2i MOUSE_POS;

        void refresh(sf::RenderWindow &renderWindow);

    private:
        bool mouseDownPrev = false;
    };

public:
    TimeInfo time;
    WindowInfo window;
    InputInfo input;
};