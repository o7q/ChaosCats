#pragma once

#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <chrono>

#include "../framework/net.hpp"
#include "../messages.hpp"
#include "../data/world.hpp"
#include "../../utils/map.hpp"
#include "../../player/player.hpp"
#include "../../game_info/game_info.hpp"

class GameServer : public net::ServerInterface<GameMessages>
{
private:
    std::unordered_map<uint32_t, Player *> serverPlayers;
    World *world = nullptr;
    std::vector<uint32_t> husks;

    std::shared_mutex playersMutex;
    std::shared_mutex worldMutex;
    std::mutex husksMutex;

    std::atomic<bool> keepTick;
    std::thread tickThread;

    GameInfo serverGameInfo;

    GameAssets &gameAssets;

    const int serverTickRate = 120;
    // tick function, will be limited to 'serverTickRate'
    void start_tick()
    {
        const std::chrono::milliseconds frameDuration(1000 / serverTickRate);
        while (keepTick.load())
        {
            std::chrono::high_resolution_clock::time_point tick_start = std::chrono::high_resolution_clock::now();

            std::chrono::high_resolution_clock::time_point start_delay_time = std::chrono::high_resolution_clock::now();
            tick();
            std::chrono::high_resolution_clock::time_point end_delay_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> tickTime = std::chrono::duration_cast<std::chrono::duration<float>>(end_delay_time - start_delay_time);

            // inject delay to force the server to run at at the set tickrate
            if (tickTime < frameDuration)
            {
                std::this_thread::sleep_for(frameDuration - tickTime);
            }

            std::chrono::high_resolution_clock::time_point tick_end = std::chrono::high_resolution_clock::now();

            serverGameInfo.time.refresh(tick_start, tick_end);
        }
    }

    void tick()
    {
        {
            std::lock_guard worldLock(worldMutex);
            world->update(serverGameInfo.time.DELTA_TIME);
        }

        updateProjectiles();
        updatePlayers();
    }

    void updatePlayers();
    void updateProjectiles();

public:
    GameServer(uint16_t nPort, GameAssets &gameAssets) : net::ServerInterface<GameMessages>(nPort), gameAssets(gameAssets)
    {
        world = new World(gameAssets.maps.TEST_MAP, 5);

        keepTick.store(true);
        tickThread = std::thread(&GameServer::start_tick, this);
    }
    ~GameServer()
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

private:
    bool onClientConnect(std::shared_ptr<net::Connection<GameMessages>> client) override;
    void onClientValidated(std::shared_ptr<net::Connection<GameMessages>> client) override;
    void onClientDisconnect(std::shared_ptr<net::Connection<GameMessages>> client) override;
    void onMessage(std::shared_ptr<net::Connection<GameMessages>> client, net::Message<GameMessages> &message) override;

    void player_connect_purgeHusks();
    void player_connect_handleRegister(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
    void player_action_handleMove(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
    void player_action_handleShoot(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
    void player_action_handleWorldModify(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
};