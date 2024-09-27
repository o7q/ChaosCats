#include "../game_server.hpp"

void GameServer::player_connect_handleRegister(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    Logger::log(Logger::LogType::Info, "gameserver", "Client reqested register, assigning ID: " + std::to_string(client->getID()));

    // read playerdata from connect packet
    PlayerData pf;
    message >> pf;
    pf.id = client->getID();
    serverPlayers.insert_or_assign(pf.id, pf);

    // send new id back to newly connected client
    net::Message<GameMessages> idPacket;
    idPacket.header.id = GameMessages::Server2C_AssignID;
    idPacket << pf.id;
    sendMessageToClient(client, idPacket);

    // send new player to all clients, including the connected client
    // this is because the client itself does not create players, the server does
    // the client sends a request to the server, and the server will allow the client to display the newly created client
    net::Message<GameMessages> newPlayerPacket;
    newPlayerPacket.header.id = GameMessages::Server2C_AddPlayer;
    newPlayerPacket << pf;
    sendMessageToAllClients(newPlayerPacket);

    // display all already connected clients to the newly connected client
    for (const auto &player : serverPlayers)
    {
        net::Message<GameMessages> allPlayersPacket;
        allPlayersPacket.header.id = GameMessages::Server2C_AddPlayer;
        allPlayersPacket << player.second;
        sendMessageToClient(client, allPlayersPacket);
    }
}

void GameServer::player_connect_purgeHusks()
{
    if (!garbageIds.empty())
    {
        for (uint32_t id : garbageIds)
        {
            net::Message<GameMessages> removePlayerMessage;
            removePlayerMessage.header.id = GameMessages::Server2C_RemovePlayer;
            removePlayerMessage << id;
            Logger::log(Logger::LogType::Info, "gameserver", "Removing: " + std::to_string(id) + " from clients");
            sendMessageToAllClients(removePlayerMessage);
        }
        garbageIds.clear();
    }
}