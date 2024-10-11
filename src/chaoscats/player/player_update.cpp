#include "player.hpp"

// this function only runs for the local player (client-side)
void Player::clientUpdate()
{
    handleMovement();
    handleCollision();

    setPosition(position.x, position.y);
    if (gameInfo.window.IS_FOCUSED)
    {
        setAim(gameInfo.input.MOUSE_POS.x, gameInfo.input.MOUSE_POS.y);
    }

    damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
    damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);

    weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
}

// this function only runs for server replicated clients
void Player::replicatedClientUpdate()
{
    damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
    damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);
    weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
}

// this function only runs on the server
void Player::serverUpdate()
{
    damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
    damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);
    weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
}