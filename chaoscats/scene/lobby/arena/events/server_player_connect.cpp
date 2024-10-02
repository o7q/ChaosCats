#include "../arena.hpp"

// Server2C_Validated
void Scene_LobbyArena::server_player_connect_handleValidate(net::Message<GameMessages> &message)
{
    net::Message<GameMessages> joinPacket;
    joinPacket.header.id = GameMessages::Client2S_Register;
    client.sendMessageToServer(joinPacket);
}

// Server2C_AssignID
void Scene_LobbyArena::server_player_connect_handleAssignID(net::Message<GameMessages> &message)
{
    message >> localId;
    Logger::log(Logger::LogType::Info, "lobby", "Recieved new id from server: " + std::to_string(localId));
}

// Server2C_AddPlayer
void Scene_LobbyArena::server_player_connect_handleAdd(net::Message<GameMessages> &message)
{
    // Player *player = new Player(windowInfo, world, ASSETS.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));

    // player->setID(playerData.id);
    // player->setPosition(playerData.position.x, playerData.position.y);

    // player->pickupWeapon(ASSETS.TEXTURES.WEAPON_SHOTGUN_TEXTURE);

    // players.insert_or_assign(playerData.id, player);

    // if (player->getPlayerData().id == localId)
    // {
    //     validated = true;
    // }

    // Logger::log(Logger::LogType::Info, "lobby", "Player joined server: " + std::to_string(player->getPlayerData().id));
}

// Server2C_RemovePlayer
void Scene_LobbyArena::server_player_connect_handleRemove(net::Message<GameMessages> &message)
{
    // uint32_t id;
    // message >> id;

    // if (mapContains(players, id))
    // {
    //     if (players[id] != nullptr)
    //     {
    //         delete players[id];
    //     }

    //     players.erase(id);
    // }
}