#include "../arena.hpp"

void Scene_LobbyArena::player_action_handleMove(net::Message<GameMessages> &message)
{
    PlayerData playerData;
    message >> playerData;

    if (!mapContains(players, playerData.id))
    {
        players[playerData.id]->setPosition(playerData.position.x, playerData.position.y);
    }
}