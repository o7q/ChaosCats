#include "game_state.hpp"

#include <SFML/Graphics.hpp>

void GameState::InputState::refresh(sf::RenderWindow &renderWindow)
{
    MOUSE_POS = sf::Mouse::getPosition(renderWindow);
    MOUSE_LMB_DOWN = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (!mouseDownPrev && MOUSE_LMB_DOWN)
    {
        MOUSE_LMB_CLICKED = true;
    }
    else
    {
        MOUSE_LMB_CLICKED = false;
    }

    mouseDownPrev = MOUSE_LMB_DOWN;
}

bool GameState::InputState::left()
{
    return false;
}

bool GameState::InputState::right()
{
    return false;
}

bool GameState::InputState::up()
{
    return false;
}

bool GameState::InputState::down()
{
    return false;
}