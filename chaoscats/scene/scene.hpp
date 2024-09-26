#pragma once

#include <SFML/Graphics.hpp>

#include "../assets/asset_manager.hpp"
#include "../window/window_info.hpp"

enum class CurrentScene
{
    None,
    MainMenu,
    Lobby
};

class Scene
{
public:
    Scene(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &ASSETS) : renderWindow(renderWindow), windowInfo(windowInfo), ASSETS(ASSETS) {}
    virtual ~Scene()
    {
    }

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
    AssetStorage &ASSETS;

    bool doInit = true;
};