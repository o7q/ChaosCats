#include "../arena.hpp"

void Scene_LobbyArena::server_player_action_handleMove(net::Message<GameMessages> &message)
{
    PlayerData playerData;
    message >> playerData;

    if (mapContains(players, playerData.id))
    {
        players[playerData.id]->updateFromPlayerData(playerData);
    }
}

void Scene_LobbyArena::server_player_action_handleModifyWorld(net::Message<GameMessages> &message)
{
    world->updateFromMessage(message);
}