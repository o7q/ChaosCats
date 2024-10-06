#pragma once

#include <thread>
#include <atomic>

#include "../scene.hpp"
#include "../../rendering/draw.hpp"

#include "../../net/game_client/game_client.hpp"
#include "../../net/game_server/game_server.hpp"

#include "arena/arena.hpp"

#include "../../utils/logger.hpp"

class Scene_Lobby : public Scene
{
public:
    Scene_Lobby(sf::RenderWindow &renderWindow, GameInfo &gameInfo, GameAssets &gameAssets, const std::vector<std::string> &netArgs) : Scene(renderWindow, gameInfo, gameAssets),
                                                                                                                                       netArgs(netArgs)
    {
        keepServer.store(false);
        // serverStarted.store(false);
        serverStarted = false;
    }
    ~Scene_Lobby()
    {
        if (lobbyScene != nullptr)
        {
            delete lobbyScene;
            lobbyScene = nullptr;
        }
    }

    CurrentScene run() override;

    void runServer(uint16_t port);

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
    const std::vector<std::string> &netArgs;

    GameClient gameClient;
    GameServer *gameServer = nullptr;

    std::atomic<bool> keepServer;
    bool serverStarted = false;
    std::mutex serverStarted_mutex;
    std::condition_variable serverStarted_cv;
    std::thread serverThread;

    Scene *lobbyScene = nullptr;
    CurrentScene currentLobbyScene = CurrentScene::None;
    CurrentScene previousLobbyScene = CurrentScene::None;

    // uint32_t localId = 0;
    // bool validated = false;
};