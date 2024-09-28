#include "../arena.hpp"

void Scene_LobbyArena::player_connect_handleValidate(net::Message<GameMessages> &message)
{
    PlayerData empty_playerData;
    net::Message<GameMessages> joinPacket;
    joinPacket.header.id = GameMessages::Client2S_Register;
    joinPacket << empty_playerData;
    client.sendMessageToServer(joinPacket);
}

void Scene_LobbyArena::player_connect_handleAssignID(net::Message<GameMessages> &message)
{
    message >> localId;
    Logger::log(Logger::LogType::Info, "lobby-net", "Recieved new id from server: " + std::to_string(localId));
}

void Scene_LobbyArena::player_connect_handleAdd(net::Message<GameMessages> &message)
{
    PlayerData pf;
    message >> pf;

    Player *player = new Player(renderWindow, world, ASSETS.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));

    player->setID(pf.id);
    player->setPosition(pf.position.x, pf.position.y);

    player->pickupWeapon(ASSETS.TEXTURES.WEAPON_SHOTGUN_TEXTURE);

    players.insert_or_assign(pf.id, player);

    if (player->getPlayerData().id == localId)
    {
        validated = true;
    }

    Logger::log(Logger::LogType::Info, "lobby-net", "Player joined server: " + std::to_string(player->getPlayerData().id));
}

void Scene_LobbyArena::player_connect_handleRemove(net::Message<GameMessages> &message)
{
    uint32_t id;
    message >> id;

    if (mapContains(players, id))
    {
        if (players[id] != nullptr)
        {
            delete players[id];
        }

        players.erase(id);
    }
}