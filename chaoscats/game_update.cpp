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
        case CurrentScene::Arena:
            scene = new Scene_Arena(renderWindow, windowInfo, gameAssets, netArgs);
            break;
        case CurrentScene::MainMenu:
            scene = new Scene_MainMenu(renderWindow, windowInfo, gameAssets);
            break;
        default:
            break;
        }
    }

    previousScene = currentScene;
}