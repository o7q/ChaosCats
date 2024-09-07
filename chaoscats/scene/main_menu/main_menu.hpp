#pragma once

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../math/collision/collision.hpp"

class Scene_MainMenu : public Scene
{
public:
    Scene_MainMenu(sf::RenderWindow &renderWindow, AssetStorage &gameAssets) : Scene(renderWindow, gameAssets) {}

    CurrentScene run() override;

private:
    float rot = 0;
};