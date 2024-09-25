#pragma once

#include <thread>
#include <thread>
#include <atomic>

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
    Scene_Arena(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &gameAssets, const std::vector<std::string> &netArgs) : Scene(renderWindow, windowInfo, gameAssets), netArgs(netArgs)
    {
        keepServer.store(false);
    }
    ~Scene_Arena()
    {
        std::cout << "delete arena" << std::endl;

        for (auto &pair : players)
        {
            std::cout << "deleting " << pair.second->getPlayerFramework().id << std::endl;
            delete pair.second;
        }
        players.clear();

        delete world;
        delete worldCamera;
    }

    CurrentScene run() override;

    void runServer();

    void closeConnection()
    {
        // disconnect client
        gameClient.disconnect();

        if (gameServer != nullptr)
        {
            gameServer->stop();

            // gracefully close server thread
            keepServer.store(false);

            // delete the server
            delete gameServer;
            gameServer = nullptr;
        }
    }

private:
    WorldCamera *worldCamera = nullptr;
    World *world = nullptr;

    const std::vector<std::string> &netArgs;

    GameClient gameClient;
    GameServer *gameServer = nullptr;

    std::atomic<bool> keepServer;
    std::thread serverThread;

    std::unordered_map<int, Player *> players;

    int localId = 0;
    bool validated = false;
};