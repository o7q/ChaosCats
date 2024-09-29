#include "game.hpp"

bool mouseDownPrev = false;

void GameWindow::handleInput()
{
    bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (!mouseDownPrev && mouseDown)
    {
        WINDOW_INFO.INPUT_INFO.MOUSE_CLICKED = true;
    }
    else
    {
        WINDOW_INFO.INPUT_INFO.MOUSE_CLICKED = false;
    }

    mouseDownPrev = mouseDown;
}