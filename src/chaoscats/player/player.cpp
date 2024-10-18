#include "player.hpp"

Player::Player(const GameState &gameState,
               const World *world,
               const GameAssets &gameAssets) : Entity(gameState, world),
                                               gameAssets(gameAssets)
{
    init();
}

Player::Player(const PlayerData &playerData,
               const GameState &gameState,
               const World *world,
               const GameAssets &gameAssets) : Entity(gameState, world),
                                               gameAssets(gameAssets)
{
    init();

    setPosition(Vector<float>(playerData.positionX, playerData.positionY));
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