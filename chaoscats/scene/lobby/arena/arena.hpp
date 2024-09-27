#pragma once

#include <unordered_map>

#include "../../scene.hpp"
#include "../../../rendering/draw.hpp"
#include "../../../math/geometry/geometry_common.hpp"

#include "../../../object/player/player.hpp"
#include "../../../world/world.hpp"
#include "../../../world/camera.hpp"

#include "../../../net/game_client/game_client.hpp"

class Scene_LobbyArena : public Scene
{
public:
    Scene_LobbyArena(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &ASSETS, GameClient &client) : Scene(renderWindow, windowInfo, ASSETS),
                                                                                                                         client(client)
    {
    }
    ~Scene_LobbyArena()
    {
        std::cout << "delete arena" << std::endl;

        // delete local serverplayers
        for (auto &pair : players)
        {
            std::cout << "deleting " << pair.second->getPlayerData().id << std::endl;
            delete pair.second;
        }
        players.clear();

        delete world;
        delete worldCamera;
    }

    CurrentScene run() override;

private:
    GameClient &client;

    WorldCamera *worldCamera = nullptr;
    World *world = nullptr;

    std::unordered_map<int, Player *> players;

    int localId = 0;
    bool validated = false;
};