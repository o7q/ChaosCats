#pragma once

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/types/types_common.hpp"

#include "../../world/world.hpp"

class Scene_MainMenu : public Scene
{
public:
    Scene_MainMenu(sf::RenderWindow &renderWindow, GameState &gameState, GameAssets &gameAssets) : Scene(renderWindow, gameState, gameAssets) {}
    ~Scene_MainMenu()
    {
    }

    CurrentScene run() override;
};