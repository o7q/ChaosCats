#pragma once

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../math/collision/collision.hpp"

#include "../../player/player.hpp"
#include "../../world/world.hpp"

class Scene_MainMenu : public Scene
{
public:
    Scene_MainMenu(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &gameAssets) : Scene(renderWindow, windowInfo, gameAssets) {}

    CurrentScene run() override;

private:
    float rot = 0;

    Player *player = nullptr;
    World *world = nullptr;
};