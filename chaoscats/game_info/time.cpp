#include "game_info.hpp"

#include <cmath>

#include <SFML/Graphics.hpp>

void GameInfo::TimeInfo::refresh(clock_t frame_start, clock_t frame_end)
{
    FRAMERATE = std::round(1 / (static_cast<float>(frame_end - frame_start) / CLOCKS_PER_SEC));
    DELTA_TIME = 60.0f / FRAMERATE;
}