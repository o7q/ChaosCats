#pragma once

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../object/player/player.hpp"
#include "../../world/world.hpp"
#include "../../world/camera.hpp"

class Scene_MainMenu : public Scene
{
public:
    Scene_MainMenu(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &gameAssets) : Scene(renderWindow, windowInfo, gameAssets) {}
    ~Scene_MainMenu()
    {
        std::cout << "delete arena" << std::endl;
    }

    CurrentScene run() override;
};