#include "../arena.hpp"

// Server2C_MovePlayer
void Scene_LobbyArena::server_player_action_handleMove(net::Message<GameMessages> &message)
{
    Vector<float> position;
    Vector<float> aimPosition;
    uint32_t id;
    message >> id >> aimPosition >> position;

    if (mapContains(players, id))
    {
        players[id]->setPosition(position.x, position.y);
        players[id]->setAim(aimPosition.x, aimPosition.y);
    }
}

// Server2C_PlayerShoot
void Scene_LobbyArena::server_player_action_handleShoot(net::Message<GameMessages> &message)
{
    uint32_t id;
    message >> id;

    if (mapContains(players, id))
    {
        players[id]->shoot();
    }
}

// Server2C_DamagePlayer
void Scene_LobbyArena::server_player_action_handleDamage(net::Message<GameMessages> &message)
{
    uint32_t id;
    ProjectileCollideInfo pci;
    message >> id >> pci;

    if (mapContains(players, id))
    {
        players[id]->damage(pci);
    }
}