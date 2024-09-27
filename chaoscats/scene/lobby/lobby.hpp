#pragma once

#include <thread>
#include <atomic>

#include "../scene.hpp"
#include "../../rendering/draw.hpp"

#include "../../net/game_client/game_client.hpp"
#include "../../net/game_server/game_server.hpp"

#include "arena/arena.hpp"

class Scene_Lobby : public Scene
{
public:
    Scene_Lobby(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &ASSETS, const std::vector<std::string> &netArgs) : Scene(renderWindow, windowInfo, ASSETS),
                                                                                                                                         netArgs(netArgs)
    {
        keepServer.store(false);
    }
    ~Scene_Lobby()
    {
        std::cout << "delete lobby" << std::endl;

        if (lobbyScene != nullptr)
        {
            delete lobbyScene;
        }
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
    const std::vector<std::string> &netArgs;

    GameClient gameClient;
    GameServer *gameServer = nullptr;

    std::atomic<bool> keepServer;
    std::thread serverThread;

    Scene *lobbyScene = nullptr;
    CurrentScene currentLobbyScene = CurrentScene::None;
    CurrentScene previousLobbyScene = CurrentScene::None;

    int localId = 0;
    bool validated = false;
};