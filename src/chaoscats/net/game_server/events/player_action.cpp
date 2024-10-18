#include "../game_server.hpp"

// Client2S_Move
void GameServer::player_action_handleMove(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    uint32_t id = client->getID();
    
    Vector<float> position;
    Vector<float> aimPosition;
    message >> aimPosition >> position;

    {
        std::lock_guard playersLock(playersMutex);
        serverPlayers[id]->setPosition(position);
        serverPlayers[id]->setAim(aimPosition.x, aimPosition.y);
    }

    net::Message<GameMessages> movePacket;
    movePacket.header.id = GameMessages::Server2C_MovePlayer;
    movePacket << position << aimPosition << id;
    sendMessageToAllClients(movePacket, client);
}

// Client2S_Shoot
void GameServer::player_action_handleShoot(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    uint32_t id = client->getID();

    {
        std::lock_guard playersLock(playersMutex);
        serverPlayers[id]->shoot();
    }

    net::Message<GameMessages> shootPacket;
    shootPacket.header.id = GameMessages::Server2C_PlayerShoot;
    shootPacket << id;
    sendMessageToAllClients(shootPacket, client);
}