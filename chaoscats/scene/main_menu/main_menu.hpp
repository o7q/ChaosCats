#pragma once

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../world/world.hpp"
#include "../../world/camera.hpp"

class Scene_MainMenu : public Scene
{
public:
    Scene_MainMenu(sf::RenderWindow &renderWindow, GameInfo &gameInfo, GameAssets &gameAssets) : Scene(renderWindow, gameInfo, gameAssets) {}
    ~Scene_MainMenu()
    {
    }

    CurrentScene run() override;
};