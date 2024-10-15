#pragma once

#include <iostream>
#include <cmath>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "scene/scene.hpp"
#include "scene/scenes_common.hpp"

#include "game_assets/game_assets.hpp"

#include "game_state/game_state.hpp"

class GameWindow
{
public:
    GameWindow(const std::string &title, int width, int height, const std::vector<std::string> &netArgs);

private:
    void update();

    sf::RenderWindow renderWindow;
    GameState gameState;

    // this is all of the games assets, this one line automatically loads all game assets
    GameAssets gameAssets;

    Scene *scene = nullptr;
    CurrentScene currentScene = CurrentScene::None;
    CurrentScene previousScene = CurrentScene::None;

    const std::vector<std::string> &netArgs;
};