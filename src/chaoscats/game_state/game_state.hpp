#pragma once

#include <chrono>
#include <utility>

#include <SFML/Graphics.hpp>

class GameState
{
public:
    class TimeState
    {
    public:
        int FRAMERATE = 0;
        float DELTA_TIME = 0.0f;
        float BASE_DELTA_TIME = 0.0f;

        void refresh(std::chrono::high_resolution_clock::time_point frame_start, std::chrono::high_resolution_clock::time_point frame_end);

        void setGameSpeed(float speed);

    private:
        float timeModifier = 1.0f;
    };

    class WindowState
    {
    public:
        float ASPECT = 1.0f;
        unsigned int WIDTH = 1;
        unsigned int HEIGHT = 1;

        bool IS_FOCUSED = true;

        float RESIZE_SCALE = 1.0f;
        float ASPECT_SCALE = 1.0f;
        float RESIZE_OFFSET_X = 0.0f;
        float RESIZE_OFFSET_Y = 0.0f;

        void refresh(sf::RenderWindow &renderWindow);

    private:
        float calculateResizeScale(const sf::Vector2u &windowSize);
        float calculateAspectScale(const sf::Vector2u &windowSize);
        std::pair<float, float> calculateResizeOffset(const sf::Vector2u &windowSize);
    };

    class InputState
    {
    public:
        bool MOUSE_LMB_CLICKED = false;
        bool MOUSE_LMB_DOWN = false;
        sf::Vector2i MOUSE_POS;

        void refresh(sf::RenderWindow &renderWindow);

        bool left();
        bool right();
        bool up();
        bool down();

    private:
        bool mouseDownPrev = false;
    };

public:
    TimeState time;
    WindowState window;
    InputState input;
};