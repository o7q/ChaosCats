#include "../arena.hpp"

void Scene_LobbyArena::client_player_action_handleMove()
{
    if (packetCanSend)
    {
        net::Message<GameMessages> movePacket;
        movePacket.header.id = GameMessages::Client2S_Move;
        movePacket << players[localId]->getPlayerData();
        client.sendMessageToServer(movePacket);
    }
}