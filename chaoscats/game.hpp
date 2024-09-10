#pragma once

#include <iostream>
#include <cmath>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "scene/scene.hpp"
#include "scene/scenes_common.hpp"

#include "assets/asset_manager.hpp"

#include "window/window_info.hpp"

class GameWindow
{
public:
    GameWindow(const std::string &title, int width, int height);

private:
    void update();

    int FRAMERATE = 0;

    sf::RenderWindow renderWindow;
    WindowInfo windowInfo;

    AssetStorage gameAssets;

    CurrentScene currentScene = CurrentScene::SCENE_MAIN_MENU;

    Scene_MainMenu *scene_mainMenu;
};