#pragma once

#include <SFML/Graphics.hpp>

#include "../assets/asset_manager.hpp"
#include "../window/window_info.hpp"

enum class CurrentScene
{
    Default,
    Arena
};

class Scene
{
public:
    Scene(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &gameAssets) : renderWindow(renderWindow), windowInfo(windowInfo), gameAssets(gameAssets) {}

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
    WindowInfo &windowInfo;
    AssetStorage &gameAssets;

    bool doInit = true;
};