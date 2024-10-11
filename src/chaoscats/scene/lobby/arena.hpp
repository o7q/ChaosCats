#pragma once

#include <cstdint>
#include <unordered_map>

#include "../scene.hpp"
#include "../../rendering/draw.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../player/player.hpp"
#include "../../world/world.hpp"
#include "../../camera/camera.hpp"

#include "../../net/game_client/game_client.hpp"

#include "../../utils/map.hpp"

class Scene_LobbyArena : public Scene
{
public:
    Scene_LobbyArena(sf::RenderWindow &renderWindow,
                     GameInfo &gameInfo,
                     GameAssets &gameAssets,
                     GameClient &client) : Scene(renderWindow,
                                                 gameInfo,
                                                 gameAssets),
                                           client(client)
    {
    }
    ~Scene_LobbyArena()
    {
        // delete local serverplayers
        for (auto &pair : players)
        {
            if (pair.second != nullptr)
            {
                delete pair.second;
                pair.second = nullptr;
            }
        }
        players.clear();

        if (world != nullptr)
        {
            delete world;
            world = nullptr;
        }
    }

    CurrentScene run() override;

private:
    GameClient &client;

    Camera camera;
    World *world = nullptr;

    std::unordered_map<uint32_t, Player *> players;

    uint32_t localId = 0;
    bool validated = false;

protected:
    void handleIncomingPackets()
    {
        if (client.isConnectedToServer())
        {
            while (!client.incoming().empty())
            {
                auto message = client.incoming().pop_front().message;

                switch (message.header.id)
                {
                case GameMessages::Server2C_Validated:
                {
                    server_player_connect_handleValidate(message);
                }
                break;
                case GameMessages::Server2C_AssignID:
                {
                    server_player_connect_handleAssignID(message);
                }
                break;
                case GameMessages::Server2C_AddPlayer:
                {
                    server_player_connect_handleAdd(message);
                }
                break;
                case GameMessages::Server2C_RemovePlayer:
                {
                    server_player_connect_handleRemove(message);
                }
                break;
                case GameMessages::Server2C_MovePlayer:
                {
                    server_player_action_handleMove(message);
                }
                break;
                case GameMessages::Server2C_DamagePlayer:
                {
                    server_player_action_handleDamage(message);
                }
                break;
                case GameMessages::Server2C_PlayerShoot:
                {
                    server_player_action_handleShoot(message);
                }
                break;
                case GameMessages::Server2C_ModifyWorld:
                {
                    server_world_action_handleModifyWorld(message);
                }
                break;
                case GameMessages::Server2C_SyncWorldStart:
                {
                    server_world_handleSyncWorldStart(message);
                }
                break;
                case GameMessages::Server2C_SyncWorldChunk:
                {
                    server_world_handleSyncWorldChunk(message);
                }
                break;
                default:
                    break;
                }
            }
        }
    }

    void server_player_connect_handleValidate(net::Message<GameMessages> &message);
    void server_player_connect_handleAssignID(net::Message<GameMessages> &message);
    void server_player_connect_handleAdd(net::Message<GameMessages> &message);
    void server_player_connect_handleRemove(net::Message<GameMessages> &message);

    void server_player_action_handleMove(net::Message<GameMessages> &message);
    void server_player_action_handleShoot(net::Message<GameMessages> &message);
    void server_player_action_handleDamage(net::Message<GameMessages> &message);

    void server_world_action_handleModifyWorld(net::Message<GameMessages> &message);
    void server_world_handleSyncWorldStart(net::Message<GameMessages> &message);
    void server_world_handleSyncWorldChunk(net::Message<GameMessages> &message);

    void client_player_action_handleMove();
    void client_player_action_handleShoot();

protected:
    const int packetSendRate = 60;
    int packetSendCounter = 0;
    bool packetCanSend = false;
};