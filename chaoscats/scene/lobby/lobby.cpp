#include "lobby.hpp"

void Scene_Lobby::runServer()
{
    while (keepServer.load())
    {
        gameServer->update(-1, true);
    }
}

CurrentScene Scene_Lobby::run()
{
    if (init())
    {
        Logger::log(Logger::LogType::Info, "lobby", "Initializing lobby");

        std::string address;
        uint16_t port;

        // if a server, host the server, then join on the localhost
        if (netArgs[0] == "host")
        {
            Logger::log(Logger::LogType::Info, "lobby", "Mode: hostserver");

            address = "127.0.0.1";
            port = std::stoi(netArgs[1]);

            gameServer = new GameServer(port);
            gameServer->start();

            keepServer.store(true);

            serverThread = std::thread(&Scene_Lobby::runServer, this);
        }

        // if a client, join the server using the address and port specified
        else if (netArgs[0] == "join")
        {
            Logger::log(Logger::LogType::Info, "lobby", "Mode: joinserver");

            address = netArgs[1];
            port = std::stoi(netArgs[2]);
        }

        Logger::log(Logger::LogType::Info, "lobby", "Joining " + address + ":" + std::to_string(port));
        gameClient.connect(address, port);

        currentLobbyScene = CurrentScene::LobbyArena;
    }

    if (update())
    {
        if (lobbyScene != nullptr)
        {
            currentLobbyScene = lobbyScene->run();
        }

        if (currentLobbyScene != previousLobbyScene)
        {
            if (lobbyScene != nullptr)
            {
                delete lobbyScene;
            }
            switch (currentLobbyScene)
            {
            case CurrentScene::LobbyArena:
                lobbyScene = new Scene_LobbyArena(renderWindow, windowInfo, ASSETS, gameClient);
                break;
            default:
                break;
            }
        }

        previousLobbyScene = currentLobbyScene;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && windowInfo.IS_FOCUSED)
        {
            terminateConnection();
            return CurrentScene::MainMenu;
        }
    }

    return CurrentScene::Lobby;
}