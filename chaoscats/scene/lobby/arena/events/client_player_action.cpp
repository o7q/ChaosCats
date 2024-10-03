#include "../arena.hpp"

void Scene_LobbyArena::client_player_action_handleMove()
{
    if (packetCanSend)
    {
        net::Message<GameMessages> movePacket;
        movePacket.header.id = GameMessages::Client2S_Move;
        movePacket << players[localId]->getPosition() << players[localId]->getAim() << localId;
        client.sendMessageToServer(movePacket);
    }
}

void Scene_LobbyArena::client_player_action_handleShoot()
{
    net::Message<GameMessages> shootPacket;
    shootPacket.header.id = GameMessages::Client2S_Shoot;
    shootPacket << localId;
    client.sendMessageToServer(shootPacket);
}