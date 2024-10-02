#include "../arena.hpp"

// Server2C_MovePlayer
void Scene_LobbyArena::server_player_action_handleMove(net::Message<GameMessages> &message)
{
    // Point<float> position;
    // uint32_t id;
    // message >> id >> position;

    // if (mapContains(players, id))
    // {
    //     players[id]->setPosition(position.x, position.y);
    // }
}

// Server2C_ModifyWorld
void Scene_LobbyArena::server_player_action_handleModifyWorld(net::Message<GameMessages> &message)
{
    // world->updateFromMessage(message);
}