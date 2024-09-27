#include "../game_server.hpp"

void GameServer::player_action_handleMove(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    PlayerData pf;
    message >> pf;

    serverPlayers[pf.id] = pf;

    net::Message<GameMessages> movePacket;
    movePacket.header.id = GameMessages::Server2C_MovePlayer;
    movePacket << pf;
    sendMessageToAllClients(movePacket, client);
}