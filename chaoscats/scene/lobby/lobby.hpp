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

class Scene_Lobby : public Scene
{
public:
    Scene_Lobby(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &ASSETS, const std::vector<std::string> &netArgs) : Scene(renderWindow, windowInfo, ASSETS), netArgs(netArgs)
    {
        keepServer.store(false);
    }
    ~Scene_Lobby()
    {
        std::cout << "delete arena" << std::endl;

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

    void runServer();

    void terminateConnection()
    {
        // disconnect client
        gameClient.disconnect();

        gameServer->stop();
        if (gameServer != nullptr)
        {
            // gracefully close server thread
            keepServer.store(false);

            if (serverThread.joinable())
            {
                serverThread.join();
            }

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