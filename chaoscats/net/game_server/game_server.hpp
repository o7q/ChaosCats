#pragma once

#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>
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
public:
    std::unordered_map<uint32_t, Player *> serverPlayers;
    World *world = nullptr;
    std::vector<uint32_t> husks;

    std::atomic<bool> keepTick;
    std::thread tickThread;
    std::mutex playersMutex;
    std::mutex worldMutex;

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
        worldMutex.lock();
        world->update();
        worldMutex.unlock();

        {
            std::lock_guard<std::mutex> playersLock(playersMutex);
            if (!serverPlayers.empty())
            {
                for (const auto &player : serverPlayers)
                {
                    player.second->serverUpdate();
                    std::vector<ProjectileCollideInfo> projectileCollideInfo = player.second->getProjectileCollideInfo();
                    for (ProjectileCollideInfo pci : projectileCollideInfo)
                    {
                        Particle particle;
                        worldMutex.lock();
                        net::Message<GameMessages> worldModifyMessage = world->drawParticles(pci.x, pci.y, particle, pci.projectileSettings.damageRadius, DrawShape::Circle);
                        worldMutex.unlock();
                        worldModifyMessage.header.id = GameMessages::Server2C_ModifyWorld;
                        sendMessageToAllClients(worldModifyMessage);
                    }
                }
            }
        }
    }

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

        for (auto &pair : serverPlayers)
        {
            if (pair.second != nullptr)
            {
                delete pair.second;
            }
        }
        serverPlayers.clear();

        if (world != nullptr)
        {
            delete world;
        }
    }

    bool onClientConnect(std::shared_ptr<net::Connection<GameMessages>> client) override
    {
        return true;
    }

    void onClientValidated(std::shared_ptr<net::Connection<GameMessages>> client) override
    {
        Logger::log(Logger::LogType::Info, "gameserver", "Client validated, informing client");
        net::Message<GameMessages> validatedMessage;
        validatedMessage.header.id = GameMessages::Server2C_Validated;
        sendMessageToClient(client, validatedMessage);
    }

    void onClientDisconnect(std::shared_ptr<net::Connection<GameMessages>> client) override
    {
        uint32_t id = client->getID();
        Logger::log(Logger::LogType::Info, "gameserver", "Client disconnected, purging: " + std::to_string(id));

        if (client != nullptr)
        {
            if (mapContains(serverPlayers, id))
            {
                if (serverPlayers[id] != nullptr)
                {
                    delete serverPlayers[id];
                }

                serverPlayers.erase(id);
                husks.push_back(id);
            }
        }
    }

    void onMessage(std::shared_ptr<net::Connection<GameMessages>> client, net::Message<GameMessages> &message) override
    {
        player_connect_purgeHusks();

        switch (message.header.id)
        {
        case GameMessages::Client2S_Register:
        {
            player_connect_handleRegister(client, message);
        }
        break;
        case GameMessages::Client2S_Move:
        {
            player_action_handleMove(client, message);
        }
        break;
        case GameMessages::Client2S_Shoot:
        {
            player_action_handleShoot(client, message);
        }
        break;
        default:
            break;
        }
    }

    //
    //
    //
    //
    //
    //
    //
    //
    //
    //

    void player_connect_purgeHusks();
    void player_connect_handleRegister(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
    void player_action_handleMove(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
    void player_action_handleShoot(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
    void player_action_handleWorldModify(std::shared_ptr<net::Connection<GameMessages>> &client, net::Message<GameMessages> &message);
};