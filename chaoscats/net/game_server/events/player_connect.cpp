#include "../game_server.hpp"

void GameServer::player_connect_handleRegister(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    uint32_t clientId = client->getID();

    Logger::log(Logger::LogType::Info, "gameserver", "Client reqested register, assigning ID: " + std::to_string(clientId));

    // read playerdata from connect packet
    PlayerData playerData;
    message >> playerData;
    playerData.id = clientId;
    serverPlayers.insert_or_assign(playerData.id, playerData);

    // send new id back to newly connected client
    net::Message<GameMessages> assignIdPacket;
    assignIdPacket.header.id = GameMessages::Server2C_AssignID;
    assignIdPacket << playerData.id;
    sendMessageToClient(client, assignIdPacket);

    // send new player to all clients, including the connected client
    // this is because the client itself does not create players, the server does
    // the client sends a request to the server, and the server will allow the client to display the newly created client
    net::Message<GameMessages> addNewPlayerPacket;
    addNewPlayerPacket.header.id = GameMessages::Server2C_AddPlayer;
    addNewPlayerPacket << playerData;
    sendMessageToAllClients(addNewPlayerPacket);

    // display all already connected clients to the newly connected client
    for (const auto &player : serverPlayers)
    {
        net::Message<GameMessages> addPastPlayersPacket;
        addPastPlayersPacket.header.id = GameMessages::Server2C_AddPlayer;
        addPastPlayersPacket << player.second;
        sendMessageToClient(client, addPastPlayersPacket);
    }
}

void GameServer::player_connect_purgeHusks()
{
    if (!husks.empty())
    {
        for (uint32_t husk_id : husks)
        {
            net::Message<GameMessages> removePlayerMessage;
            removePlayerMessage.header.id = GameMessages::Server2C_RemovePlayer;
            removePlayerMessage << husk_id;
            Logger::log(Logger::LogType::Info, "gameserver", "Removing: " + std::to_string(husk_id) + " from clients");
            sendMessageToAllClients(removePlayerMessage);
        }
        husks.clear();
    }
}