#include "game.hpp"

void GameWindow::update()
{
    switch (currentScene)
    {
    case CurrentScene::Arena:
        currentScene = scene_arena->run();
        break;
    default:
        break;
    }
}