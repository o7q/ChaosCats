#include "../game_server.hpp"

// Client2S_Register
void GameServer::player_connect_handleRegister(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message)
{
    // send world to newly connected client
    {
        std::shared_lock<std::shared_mutex> worldLock(worldMutex);

        Vector<int> worldSize = world->getSize();

        float maxPacketBytes = 1000 / static_cast<float>(sizeof(ParticleData));
        int chunkSize = std::sqrt(maxPacketBytes);

        int operations = std::ceil(worldSize.x / static_cast<float>(chunkSize)) * std::ceil(worldSize.y / static_cast<float>(chunkSize));

        net::Message<GameMessages> worldSyncStartPacket;
        worldSyncStartPacket.header.id = GameMessages::Server2C_SyncWorldStart;
        worldSyncStartPacket << world->getSize() << world->getPixelSize() << operations;
        sendMessageToClient(client, worldSyncStartPacket);

        for (int y = 0; y < worldSize.y; y += chunkSize)
        {
            for (int x = 0; x < worldSize.x; x += chunkSize)
            {
                int buffer_sizeX = (x + chunkSize > worldSize.x) ? (worldSize.x - x) : chunkSize;
                int buffer_sizeY = (y + chunkSize > worldSize.y) ? (worldSize.y - y) : chunkSize;

                Matrix<Particle> worldBuffer(buffer_sizeY, buffer_sizeX);

                net::Message<GameMessages> bufferMessage;
                bufferMessage.header.id = GameMessages::Server2C_SyncWorldChunk;

                for (int buffer_y = 0; buffer_y < buffer_sizeY; ++buffer_y)
                {
                    for (int buffer_x = 0; buffer_x < buffer_sizeX; ++buffer_x)
                    {
                        ParticleData pd;
                        pd.particle = world->getParticle(x + buffer_x, y + buffer_y);
                        pd.worldX = x + buffer_x;
                        pd.worldY = y + buffer_y;

                        bufferMessage << pd;
                    }
                }

                bufferMessage << buffer_sizeX * buffer_sizeY;

                sendMessageToClient(client, bufferMessage);
            }
        }
    }

    uint32_t clientId = client->getID();

    Logger::log(Logger::LogType::Info, "gameserver", "Client reqested register, assigning ID: " + std::to_string(clientId));

    // add new player to the server player list
    Player *player = new Player(world, serverGameInfo, gameAssets);

    {
        std::lock_guard<std::shared_mutex> playersLock(playersMutex);
        serverPlayers.insert_or_assign(clientId, player);
    }

    // send new id back to newly connected client
    net::Message<GameMessages> assignIdPacket;
    assignIdPacket.header.id = GameMessages::Server2C_AssignID;
    assignIdPacket << clientId;
    sendMessageToClient(client, assignIdPacket);

    // send new player to all clients, including the connected client
    // this is because the client itself does not create players, the server does
    // the client sends a request to the server, and the server will allow the client to display the newly created client
    net::Message<GameMessages> addNewPlayerPacket;
    addNewPlayerPacket.header.id = GameMessages::Server2C_AddPlayer;
    addNewPlayerPacket << Vector<float>(0, 0) << clientId;
    sendMessageToAllClients(addNewPlayerPacket);

    // display all already connected clients to the newly connected client
    {
        std::shared_lock<std::shared_mutex> playersLock(playersMutex);
        for (const auto &player : serverPlayers)
        {
            // skip duplicating newly connected client
            if (player.first == clientId)
            {
                continue;
            }

            net::Message<GameMessages> addPastPlayersPacket;
            addPastPlayersPacket.header.id = GameMessages::Server2C_AddPlayer;
            addPastPlayersPacket << player.second->getPosition() << player.first;
            sendMessageToClient(client, addPastPlayersPacket);
        }
    }
}

void GameServer::player_connect_purgeHusks()
{
    std::lock_guard<std::shared_mutex> playersLock(playersMutex);
    std::lock_guard<std::mutex> husksLock(husksMutex);

    // purge all players that are considered a 'husk', a serverPlayer with no corresponding client
    if (!husks.empty())
    {
        for (uint32_t husk_id : husks)
        {
            net::Message<GameMessages> removePlayerMessage;
            removePlayerMessage.header.id = GameMessages::Server2C_RemovePlayer;
            removePlayerMessage << husk_id;

            if (mapContains(serverPlayers, husk_id))
            {
                if (serverPlayers[husk_id] != nullptr)
                {
                    delete serverPlayers[husk_id];
                }

                serverPlayers.erase(husk_id);
            }

            Logger::log(Logger::LogType::Info, "gameserver", "Removing: " + std::to_string(husk_id) + " from clients");
            sendMessageToAllClients(removePlayerMessage);
        }
        husks.clear();
    }
}