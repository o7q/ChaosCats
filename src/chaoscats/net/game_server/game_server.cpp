#include "game_server.hpp"

GameServer::GameServer(uint16_t nPort, GameAssets &gameAssets) : net::ServerInterface<GameMessages>(nPort), gameAssets(gameAssets)
{
    world = new World(gameAssets.maps.TEST_MAP, 5, true);

    keepTick.store(true);
    tickThread = std::thread(&GameServer::start_tick, this);
}

GameServer::~GameServer()
{
    keepTick.store(false);
    if (tickThread.joinable())
    {
        tickThread.join();
    }

    player_connect_purgeHusks();

    {
        std::lock_guard playersLock(playersMutex);
        for (auto &pair : serverPlayers)
        {
            if (pair.second != nullptr)
            {
                delete pair.second;
                pair.second = nullptr;
            }
        }
        serverPlayers.clear();
    }

    {
        std::lock_guard worldLock(worldMutex);
        if (world != nullptr)
        {
            delete world;
            world = nullptr;
        }
    }
}