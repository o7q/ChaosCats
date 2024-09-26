#include "game_server.hpp"

#include "../packets/player.hpp"

void GameServer::handlePlayerRegister(std::shared_ptr<jnet::Connection<GameMessages>> &client, jnet::Message<GameMessages> &message)
{
    Logger::log(Logger::LogType::Info, "gameserver", "Client reqested register, assigning ID: " + std::to_string(client->getID()));

    // read playerdata from connect packet
    PlayerData pf;
    message >> pf;
    pf.id = client->getID();
    serverPlayers.insert_or_assign(pf.id, pf);

    // send new id back to newly connected client
    jnet::Message<GameMessages> idPacket;
    idPacket.header.id = GameMessages::Server2C_AssignID;
    idPacket << pf.id;
    sendMessageToClient(client, idPacket);

    // send new player to all clients, including the connected client
    // this is because the client itself does not create players, the server does
    // the client sends a request to the server, and the server will allow the client to display the newly created client
    jnet::Message<GameMessages> newPlayerPacket;
    newPlayerPacket.header.id = GameMessages::Server2C_AddPlayer;
    newPlayerPacket << pf;
    sendMessageToAllClients(newPlayerPacket);

    // display all already connected clients to the newly connected client
    for (const auto &player : serverPlayers)
    {
        jnet::Message<GameMessages> allPlayersPacket;
        allPlayersPacket.header.id = GameMessages::Server2C_AddPlayer;
        allPlayersPacket << player.second;
        sendMessageToClient(client, allPlayersPacket);
    }
}

void GameServer::handlePlayerMove(std::shared_ptr<jnet::Connection<GameMessages>> &client, jnet::Message<GameMessages> &message)
{
    PlayerData pf;
    message >> pf;

    serverPlayers[pf.id] = pf;

    jnet::Message<GameMessages> movePacket;
    movePacket.header.id = GameMessages::Server2C_MovePlayer;
    movePacket << pf;
    sendMessageToAllClients(movePacket, client);
}