#pragma once

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../object/player/player.hpp"
#include "../../world/world.hpp"
#include "../../world/camera.hpp"

class Scene_Arena : public Scene
{
public:
    Scene_Arena(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &gameAssets) : Scene(renderWindow, windowInfo, gameAssets) {}
    ~Scene_Arena()
    {
        std::cout << "delete arena" << std::endl;

        delete player;
        delete worldCamera;
        delete world;
    }

    CurrentScene run() override;

private:
    Player *player = nullptr;
    WorldCamera *worldCamera = nullptr;
    World *world = nullptr;
};