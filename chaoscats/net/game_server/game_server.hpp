#pragma once

#include <iostream>
#include <unordered_map>

#include "../framework/net.hpp"
#include "../messages.hpp"
#include "../data/player.hpp"
#include "../../utils/map.hpp"

class GameServer : public net::ServerInterface<GameMessages>
{
public:
    std::unordered_map<uint32_t, PlayerData> serverPlayers;
    std::vector<uint32_t> husks;

    GameServer(uint16_t nPort) : net::ServerInterface<GameMessages>(nPort)
    {
    }
    ~GameServer()
    {
    }

    bool onClientConnect(std::shared_ptr<net::Connection<GameMessages>> client) override
    {
        return true;
    }

    void onClientValidated(std::shared_ptr<net::Connection<GameMessages>> client) override
    {
        Logger::log(Logger::LogType::Info, "gameserver", "Client validated, sending new ID");
        net::Message<GameMessages> validatedMessage;
        validatedMessage.header.id = GameMessages::Server2C_Validated;
        sendMessageToClient(client, validatedMessage);
    }

    void onClientDisconnect(std::shared_ptr<net::Connection<GameMessages>> client) override
    {
        uint32_t id = client->getID();
        Logger::log(Logger::LogType::Info, "gameserver", "Client disconnected, purging: " + std::to_string(id));

        if (client != nullptr)
        {
            if (mapContains(serverPlayers, id))
            {
                serverPlayers.erase(id);
                husks.push_back(id);
            }
        }
    }

    void onMessage(std::shared_ptr<net::Connection<GameMessages>> client, net::Message<GameMessages> &message) override
    {
        player_connect_purgeHusks();

        switch (message.header.id)
        {
        case GameMessages::Client2S_Register:
        {
            player_connect_handleRegister(client, message);
        }
        break;
        case GameMessages::Client2S_Move:
        {
            player_action_handleMove(client, message);
        }
        break;
        default:
            break;
        }
    }

    //
    //
    //
    //
    //
    //
    //
    //
    //
    //

    void player_connect_purgeHusks();
    void player_connect_handleRegister(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
    void player_action_handleMove(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
};