#pragma once

#include <SFML/Graphics.hpp>

#include "../assets/asset_manager.hpp"

enum class CurrentScene
{
    DEFAULT,
    SCENE_MAIN_MENU
};

class Scene
{
public:
    Scene(sf::RenderWindow &renderWindow, AssetStorage &gameAssets) : renderWindow(renderWindow), gameAssets(gameAssets) {}

    virtual CurrentScene run() = 0;

protected:
    bool init()
    {
        bool init = doInit;
        doInit = false;
        return init;
    }

    bool update()
    {
        return true;
    }

    sf::RenderWindow &renderWindow;
    AssetStorage &gameAssets;

    bool doInit = true;
};