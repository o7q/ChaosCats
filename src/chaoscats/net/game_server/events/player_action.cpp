#include "../game_server.hpp"

// Client2S_Move
void GameServer::player_action_handleMove(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    uint32_t id;
    Vector<float> position;
    Vector<float> aimPosition;
    message >> id >> aimPosition >> position;

    playersMutex.lock();
    serverPlayers[id]->setPosition(position.x, position.y);
    serverPlayers[id]->setAim(aimPosition.x, aimPosition.y);
    playersMutex.unlock();

    net::Message<GameMessages> movePacket;
    movePacket.header.id = GameMessages::Server2C_MovePlayer;
    movePacket << position << aimPosition << id;
    sendMessageToAllClients(movePacket, client);
}

// Client2S_Shoot
void GameServer::player_action_handleShoot(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    uint32_t id;
    message >> id;

    playersMutex.lock();
    serverPlayers[id]->shoot();
    playersMutex.unlock();

    net::Message<GameMessages> shootPacket;
    shootPacket.header.id = GameMessages::Server2C_Shoot;
    shootPacket << id;
    sendMessageToAllClients(shootPacket, client);
}