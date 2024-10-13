#include "game_server.hpp"

bool GameServer::onClientConnect(std::shared_ptr<net::Connection<GameMessages>> client)
{
    return true;
}

void GameServer::onClientValidated(std::shared_ptr<net::Connection<GameMessages>> client)
{
    Logger::log(Logger::LogType::Info, "gameserver", "Client validated, informing client");
    net::Message<GameMessages> validatedMessage;
    validatedMessage.header.id = GameMessages::Server2C_Validated;
    sendMessageToClient(client, validatedMessage);
}

void GameServer::onClientDisconnect(std::shared_ptr<net::Connection<GameMessages>> client)
{
    if (client != nullptr)
    {
        uint32_t id = client->getID();
        Logger::log(Logger::LogType::Info, "gameserver", "Client disconnected, purging: " + std::to_string(id));

        bool playerExists = false;

        {
            std::shared_lock<std::shared_mutex> playersLock(playersMutex);
            playerExists = mapContains(serverPlayers, id);
        }

        if (playerExists)
        {
            std::lock_guard<std::mutex> husksLock(husksMutex);
            husks.push_back(id);
        }
    }
}

void GameServer::onMessage(std::shared_ptr<net::Connection<GameMessages>> client, net::Message<GameMessages> &message)
{
    player_connect_purgeHusks();

    switch (message.header.id)
    {
    case GameMessages::Client2S_Register:
    {
        player_connect_handleRegister(client, message);
    }
    break;
    case GameMessages::Client2S_PlayerMove:
    {
        player_action_handleMove(client, message);
    }
    break;
    case GameMessages::Client2S_PlayerShoot:
    {
        player_action_handleShoot(client, message);
    }
    break;
    default:
        break;
    }
}