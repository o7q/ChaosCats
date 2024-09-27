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
    GameWindow(const std::string &title, int width, int height, const std::vector<std::string> &netArgs);

private:
    void update();

    sf::RenderWindow renderWindow;
    sf::View view;
    WindowInfo windowInfo;

    AssetStorage ASSETS;

    Scene *scene = nullptr;
    CurrentScene currentScene = CurrentScene::None;
    CurrentScene previousScene = CurrentScene::None;

    const std::vector<std::string> &netArgs;
};