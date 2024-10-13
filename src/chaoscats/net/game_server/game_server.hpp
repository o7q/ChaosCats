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
public:
    GameServer(uint16_t nPort, GameAssets &gameAssets);
    ~GameServer();

private:
    World *world = nullptr;
    std::unordered_map<uint32_t, Player *> serverPlayers;
    std::vector<uint32_t> husks;

    std::shared_mutex worldMutex;
    std::shared_mutex playersMutex;
    std::mutex husksMutex;

    std::atomic<bool> keepTick;
    std::thread tickThread;

    GameInfo serverGameInfo;

    GameAssets &gameAssets;

    const int serverTickRate = 120;
    // tick function, will be limited to 'serverTickRate'
    void start_tick();
    void tick();

    void updatePlayers();
    void updateProjectiles();

private:
    // event handlers
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