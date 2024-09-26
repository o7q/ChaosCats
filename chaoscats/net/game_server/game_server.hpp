#pragma once

#include <iostream>
#include <unordered_map>

#include "../framework/net.hpp"
#include "../packets.hpp"
#include "../objects/player_framework.hpp"

class GameServer : public jnet::ServerInterface<GamePackets>
{
public:
    std::unordered_map<int, PlayerFramework> serverPlayers;

    GameServer(uint16_t nPort) : jnet::ServerInterface<GamePackets>(nPort)
    {
    }
    ~GameServer()
    {
    }

    bool onClientConnect(std::shared_ptr<jnet::Connection<GamePackets>> client) override
    {
        return true;
    }

    void onClientValidated(std::shared_ptr<jnet::Connection<GamePackets>> client) override
    {
        Logger::log(Logger::LogType::Info, "gameserver", "Client validated, sending new ID");
        jnet::Message<GamePackets> validatedMessage;
        validatedMessage.header.id = GamePackets::Server2C_Validated;
        sendMessageToClient(client, validatedMessage);
    }

    void onClientDisconnect(std::shared_ptr<jnet::Connection<GamePackets>> client) override
    {
        Logger::log(Logger::LogType::Info, "gameserver", "Client disconnected, purging: " + std::to_string(client->getID()));
    }

    void onMessage(std::shared_ptr<jnet::Connection<GamePackets>> client, jnet::Message<GamePackets> &message) override
    {
        switch (message.header.id)
        {
        case GamePackets::Client2S_Register:
        {
            handlePlayerRegister(client, message);
        }
        break;
        case GamePackets::Client2S_Move:
        {
            handlePlayerMove(client, message);
        }
        break;
        }
    }

    void handlePlayerRegister(std::shared_ptr<jnet::Connection<GamePackets>> &client, jnet::Message<GamePackets> &message);
    void handlePlayerMove(std::shared_ptr<jnet::Connection<GamePackets>> &client, jnet::Message<GamePackets> &message);
};