#include "game.hpp"

void GameWindow::update()
{
    switch (currentScene)
    {
    case CurrentScene::SCENE_MAIN_MENU:
        currentScene = scene_mainMenu->run();
        break;
    default:
        break;
    }
}