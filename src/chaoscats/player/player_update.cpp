#include "player.hpp"

// this function only runs for the local player (client-side)
void Player::clientUpdate(Camera camera)
{
    if (isDead())
    {
        return;
    }

    // determine if the player is facing right based off of their aim position
    facingRight = aimPosition.x > position.x;

    handleMovement();
    handleCollision();

    setPosition(position.x, position.y);
    if (gameState.window.IS_FOCUSED)
    {
        sf::Vector2f worldMouse = point_screenToWorld(sf::Vector2f(gameState.input.MOUSE_POS.x, gameState.input.MOUSE_POS.y), &camera, gameState.window);
        setAim(worldMouse.x, worldMouse.y);
    }

    damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
    damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);

    weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
}

// this function only runs for server replicated clients
void Player::replicatedClientUpdate()
{
    if (isDead())
    {
        return;
    }

    damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
    damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);
    weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
}

// this function only runs on the server
void Player::serverUpdate()
{
    if (isDead())
    {
        return;
    }

    damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
    damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);
    weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
}