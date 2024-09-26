#pragma once

#include <iostream>
#include <unordered_map>

#include "../framework/net.hpp"
#include "../messages.hpp"
#include "../packets/player.hpp"

class GameServer : public jnet::ServerInterface<GameMessages>
{
public:
    std::unordered_map<int, PlayerData> serverPlayers;

    GameServer(uint16_t nPort) : jnet::ServerInterface<GameMessages>(nPort)
    {
    }
    ~GameServer()
    {
    }

    bool onClientConnect(std::shared_ptr<jnet::Connection<GameMessages>> client) override
    {
        return true;
    }

    void onClientValidated(std::shared_ptr<jnet::Connection<GameMessages>> client) override
    {
        Logger::log(Logger::LogType::Info, "gameserver", "Client validated, sending new ID");
        jnet::Message<GameMessages> validatedMessage;
        validatedMessage.header.id = GameMessages::Server2C_Validated;
        sendMessageToClient(client, validatedMessage);
    }

    void onClientDisconnect(std::shared_ptr<jnet::Connection<GameMessages>> client) override
    {
        Logger::log(Logger::LogType::Info, "gameserver", "Client disconnected, purging: " + std::to_string(client->getID()));
    }

    void onMessage(std::shared_ptr<jnet::Connection<GameMessages>> client, jnet::Message<GameMessages> &message) override
    {
        switch (message.header.id)
        {
        case GameMessages::Client2S_Register:
        {
            handlePlayerRegister(client, message);
        }
        break;
        case GameMessages::Client2S_Move:
        {
            handlePlayerMove(client, message);
        }
        break;
        }
    }

    void handlePlayerRegister(std::shared_ptr<jnet::Connection<GameMessages>> &client, jnet::Message<GameMessages> &message);
    void handlePlayerMove(std::shared_ptr<jnet::Connection<GameMessages>> &client, jnet::Message<GameMessages> &message);
};