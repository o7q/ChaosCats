#pragma once

#include "framework/net.hpp"
#include "packets.hpp"

class GameClient : public jnet::ClientInterface<GamePackets>
{
public:
    void sendMovePacket(int id, float posX, float posY)
    {
        jnet::Message<GamePackets> movePacket;
        movePacket.header.id = GamePackets::Client2S_Move;
        movePacket << id << posX << posY;
        sendMessageToServer(movePacket);
    }
};