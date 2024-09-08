#pragma once

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../math/collision/collision.hpp"

#include "../../player/player.hpp"

class Scene_MainMenu : public Scene
{
public:
    Scene_MainMenu(sf::RenderWindow &renderWindow, AssetStorage &gameAssets) : Scene(renderWindow, gameAssets) {}

    CurrentScene run() override;

    Player* player = nullptr;

private:
    float rot = 0;
};