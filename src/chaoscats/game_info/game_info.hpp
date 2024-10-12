#pragma once

#include <chrono>
#include <utility>

#include <SFML/Graphics.hpp>

class GameInfo
{
public:
    struct TimeInfo
    {
        int FRAMERATE = 0;
        float DELTA_TIME = 0;

        void refresh(std::chrono::high_resolution_clock::time_point frame_start, std::chrono::high_resolution_clock::time_point frame_end);
    };

    struct WindowInfo
    {
        float ASPECT = 1.0f;
        unsigned int WIDTH = 1;
        unsigned int HEIGHT = 1;

        bool IS_FOCUSED = true;

        float RESIZE_SCALE = 1.0f;
        float ASPECT_SCALE = 1.0f;
        float RESIZE_OFFSET_X = 0.0f;
        float RESIZE_OFFSET_Y = 0.0f;

        void refresh(sf::RenderWindow &renderWindow);

        float calculateResizeScale(const sf::Vector2u &windowSize);
        float calculateAspectScale(const sf::Vector2u &windowSize);
        std::pair<float, float> calculateResizeOffset(const sf::Vector2u &windowSize);
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