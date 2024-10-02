#include "../game_server.hpp"

// Client2S_Move
void GameServer::player_action_handleMove(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    // PlayerData playerData;
    // message >> playerData;

    // serverPlayers[playerData.id] = playerData;

    // net::Message<GameMessages> movePacket;
    // movePacket.header.id = GameMessages::Server2C_MovePlayer;
    // movePacket << playerData;
    // sendMessageToAllClients(movePacket, client);
}

// Client2S_Shoot
void GameServer::player_action_handleShoot(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    // uint32_t id;
    // message >> id;

    // serverPlayers[playerData.id] = playerData;

    // net::Message<GameMessages> movePacket;
    // movePacket.header.id = GameMessages::Server2C_MovePlayer;
    // movePacket << playerData;
    // sendMessageToAllClients(movePacket, client);
}

// Client2S_ModifyWorld
void GameServer::player_action_handleWorldModify(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    // net::Message<GameMessages> worldModifyPacket = message;
    // worldModifyPacket.header.id = GameMessages::Server2C_ModifyWorld;

    // sendMessageToAllClients(worldModifyPacket, client);
}