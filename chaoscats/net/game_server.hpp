#pragma once

#include <iostream>
#include <unordered_map>

#include "framework/net.hpp"
#include "packets.hpp"
#include "../object/player/player_framework.hpp"

class GameServer : public jnet::ServerInterface<GamePackets>
{
public:
    std::unordered_map<int, PlayerFramework> serverPlayers;

    GameServer(uint16_t nPort) : jnet::ServerInterface<GamePackets>(nPort)
    {
    }

    bool onClientConnect(std::shared_ptr<jnet::Connection<GamePackets>> client) override
    {
        return true;
    }

    void onClientValidated(std::shared_ptr<jnet::Connection<GamePackets>> client) override
    {
    }

    void onClientDisconnect(std::shared_ptr<jnet::Connection<GamePackets>> client) override
    {
        std::cout << "Purging " << client->getID() << std::endl;
    }

    void onMessage(std::shared_ptr<jnet::Connection<GamePackets>> client, jnet::Message<GamePackets> &message) override
    {
        switch (message.header.id)
        {
        case GamePackets::Client2S_Connect:
            handleConnect(client, message);
            break;
        }
    }

    void handleConnect(std::shared_ptr<jnet::Connection<GamePackets>> &client, jnet::Message<GamePackets> &message);
};