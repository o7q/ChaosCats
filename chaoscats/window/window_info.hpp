#pragma once

struct InputInfo
{
    bool MOUSE_CLICKED = false;
};

struct WindowInfo
{
    int FRAMERATE = 0;
    float DELTA_TIME = 0;

    float ASPECT = 1.0f;
    unsigned int WIDTH = 1;
    unsigned int HEIGHT = 1;

    bool IS_FOCUSED = true;

    InputInfo INPUT_INFO;
};