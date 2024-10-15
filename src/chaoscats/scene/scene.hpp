#pragma once

#include <SFML/Graphics.hpp>

#include "../game_assets/game_assets.hpp"
#include "../game_state/game_state.hpp"

enum class CurrentScene
{
    None,
    MainMenu,
    Lobby,

    LobbyArena
};

class Scene
{
public:
    Scene(sf::RenderWindow &renderWindow, GameState &gameState, GameAssets &gameAssets) : renderWindow(renderWindow), gameState(gameState), gameAssets(gameAssets) {}
    virtual ~Scene()
    {
    }

    virtual void dispose()
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
    GameState &gameState;
    GameAssets &gameAssets;

    bool doInit = true;
};