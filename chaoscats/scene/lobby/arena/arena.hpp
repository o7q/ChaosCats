#pragma once

#include <cstdint>
#include <unordered_map>

#include "../../scene.hpp"
#include "../../../rendering/draw.hpp"
#include "../../../math/geometry/geometry_common.hpp"

#include "../../../player/player.hpp"
#include "../../../world/world.hpp"
#include "../../../world/camera.hpp"

#include "../../../net/game_client/game_client.hpp"

#include "../../../utils/map.hpp"

class Scene_LobbyArena : public Scene
{
public:
    Scene_LobbyArena(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, AssetStorage &ASSETS, GameClient &client) : Scene(renderWindow, windowInfo, ASSETS),
                                                                                                                         client(client)
    {
    }
    ~Scene_LobbyArena()
    {
        // delete local serverplayers
        for (auto &pair : players)
        {
            delete pair.second;
        }
        players.clear();

        delete world;
        delete worldCamera;
    }

    CurrentScene run() override;

private:
    GameClient &client;

    WorldCamera *worldCamera = nullptr;
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
                case GameMessages::Server2C_ModifyWorld:
                {
                    server_player_action_handleModifyWorld(message);
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
    void server_player_action_handleModifyWorld(net::Message<GameMessages> &message);

    void client_player_action_handleMove();

protected:
    const int packetSendRate = 60;
    int packetSendCounter = 0;
    bool packetCanSend = false;
};