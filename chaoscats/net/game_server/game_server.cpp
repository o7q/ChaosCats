#include "game_server.hpp"

#include "../objects/player_framework.hpp"

void GameServer::handlePlayerRegister(std::shared_ptr<jnet::Connection<GamePackets>> &client, jnet::Message<GamePackets> &message)
{
    std::cout << "Client requesting register, assigning ID: " << client->getID() << std::endl;

    // read playerdata from connect packet
    PlayerFramework pf;
    message >> pf;
    pf.id = client->getID();
    serverPlayers.insert_or_assign(pf.id, pf);

    // send new id back to newly connected client
    jnet::Message<GamePackets> idPacket;
    idPacket.header.id = GamePackets::Server2C_AssignID;
    idPacket << pf.id;
    sendMessageToClient(client, idPacket);

    // send new player to all clients, including the connected client
    // this is because the client itself does not create players, the server does
    // the client sends a request to the server, and the server will allow the client to display the newly created client
    jnet::Message<GamePackets> newPlayerPacket;
    newPlayerPacket.header.id = GamePackets::Server2C_AddPlayer;
    newPlayerPacket << pf;
    sendMessageToAllClients(newPlayerPacket);

    // display all already connected clients to the newly connected client
    for (const auto &player : serverPlayers)
    {
        jnet::Message<GamePackets> allPlayersPacket;
        allPlayersPacket.header.id = GamePackets::Server2C_AddPlayer;
        allPlayersPacket << player.second;
        sendMessageToClient(client, allPlayersPacket);
    }
}

void GameServer::handlePlayerMove(std::shared_ptr<jnet::Connection<GamePackets>> &client, jnet::Message<GamePackets> &message)
{
    PlayerFramework pf;
    message >> pf;

    serverPlayers[pf.id] = pf;

    jnet::Message<GamePackets> movePacket;
    movePacket.header.id = GamePackets::Server2C_MovePlayer;
    movePacket << pf;
    sendMessageToAllClients(movePacket, client);
}