#pragma once

#include <thread>

#include <unordered_map>

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../object/player/player.hpp"
#include "../../world/world.hpp"
#include "../../world/camera.hpp"

#include "../../net/game_client/game_client.hpp"
#include "../../net/game_server/game_server.hpp"

class Scene_Arena : public Scene
{
public:
    Scene_Arena(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &gameAssets, const std::vector<std::string> &netArgs) : Scene(renderWindow, windowInfo, gameAssets), netArgs(netArgs) {}
    ~Scene_Arena()
    {
        std::cout << "delete arena" << std::endl;

        // delete player;
        delete worldCamera;
        delete world;

        // close server thread
        keepServer = false;
        if (serverThread.joinable())
        {
            std::cout << "terminate server thread" << std::endl;
            serverThread.join();
            gameServer->stop();
        }
    }

    CurrentScene run() override;

    void runServer();

private:
    WorldCamera *worldCamera = nullptr;
    World *world = nullptr;

    const std::vector<std::string> &netArgs;

    GameClient gameClient;
    GameServer *gameServer = nullptr;

    bool keepServer = false;
    std::thread serverThread;

    std::unordered_map<int, Player*> players;

    int localId = 0;
    bool validated = false;
};