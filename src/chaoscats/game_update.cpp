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
            scene = nullptr;
        }
        switch (currentScene)
        {
        case CurrentScene::Lobby:
            scene = new Scene_Lobby(renderWindow, gameState, gameAssets, netArgs);
            break;
        case CurrentScene::MainMenu:
            scene = new Scene_MainMenu(renderWindow, gameState, gameAssets);
            break;
        default:
            break;
        }
    }

    previousScene = currentScene;
}