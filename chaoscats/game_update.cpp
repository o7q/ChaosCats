#include "game.hpp"

void GameWindow::update()
{
    if (scene != nullptr)
    {
        currentScene = scene->run();
    }

    if (currentScene != previousScene)
    {
        if (scene != nullptr)
        {
            delete scene;
        }
        switch (currentScene)
        {
        case CurrentScene::Lobby:
            scene = new Scene_Lobby(renderWindow, WINDOW_INFO, ASSETS, netArgs);
            break;
        case CurrentScene::MainMenu:
            scene = new Scene_MainMenu(renderWindow, WINDOW_INFO, ASSETS);
            break;
        default:
            break;
        }
    }

    previousScene = currentScene;
}