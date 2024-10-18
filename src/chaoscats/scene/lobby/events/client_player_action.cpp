#include "../arena.hpp"

// Client2S_PlayerMove
void Scene_LobbyArena::client_player_action_handleMove()
{
    if (packetCanSend)
    {
        net::Message<GameMessages> movePacket;
        movePacket.header.id = GameMessages::Client2S_PlayerMove;
        movePacket << players[localId]->getPosition() << players[localId]->getAim();
        client.sendMessageToServer(movePacket);
    }
}

// Client2S_PlayerShoot
void Scene_LobbyArena::client_player_action_handleShoot()
{
    net::Message<GameMessages> shootPacket;
    shootPacket.header.id = GameMessages::Client2S_PlayerShoot;
    client.sendMessageToServer(shootPacket);
}