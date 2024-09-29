#include "../game_server.hpp"

void GameServer::player_action_handleMove(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    PlayerData playerData;
    message >> playerData;

    serverPlayers[playerData.id] = playerData;

    net::Message<GameMessages> movePacket;
    movePacket.header.id = GameMessages::Server2C_MovePlayer;
    movePacket << playerData;
    sendMessageToAllClients(movePacket, client);
}

void GameServer::player_action_handleWorldModify(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    net::Message<GameMessages> worldModifyPacket = message;
    worldModifyPacket.header.id = GameMessages::Server2C_ModifyWorld;

    sendMessageToAllClients(worldModifyPacket, client);
}