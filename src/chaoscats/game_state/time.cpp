#include "game_state.hpp"

#include <cmath>

#include <SFML/Graphics.hpp>

void GameState::TimeState::refresh(std::chrono::high_resolution_clock::time_point frame_start, std::chrono::high_resolution_clock::time_point frame_end)
{
    std::chrono::duration<float> frame_duration = std::chrono::duration_cast<std::chrono::duration<float>>(frame_end - frame_start);
    FRAMERATE = std::round(1.0f / frame_duration.count());
    DELTA_TIME = std::clamp((60.0f / FRAMERATE) * timeModifier, 0.0f, 3.0f);
    BASE_DELTA_TIME = std::clamp(60.0f / FRAMERATE, 0.0f, 3.0f);
}

void GameState::TimeState::setGameSpeed(float speed)
{
    this->timeModifier = speed;
}