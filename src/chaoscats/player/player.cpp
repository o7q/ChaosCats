#include "player.hpp"

Player::Player(const World *world,
               GameState &gameState,
               GameAssets &gameAssets) : world(world),
                                         gameState(gameState),
                                         gameAssets(gameAssets)
{
    init();
}

Player::Player(PlayerData playerData,
               const World *world,
               GameState &gameState,
               GameAssets &gameAssets) : world(world),
                                         gameState(gameState),
                                         gameAssets(gameAssets)
{
    init();

    setPosition(playerData.positionX, playerData.positionY);
    health = playerData.health;
}

Player::~Player()
{
    if (weapon != nullptr)
    {
        delete weapon;
        weapon = nullptr;
    }
}