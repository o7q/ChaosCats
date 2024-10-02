#include "player.hpp"

void Player::handleWeapons()
{
    projectileCollideInfo.clear();
    projectileCollideInfo = weapon->update(position.x, position.y, gameInfo.input.MOUSE_POS.x, gameInfo.input.MOUSE_POS.y);

    Point<float> newVel = weapon->shoot();

    velocity.x += newVel.x;
    velocity.y += newVel.y;
}

const std::vector<ProjectileCollideInfo> &Player::getProjectileCollideInfo() const
{
    return projectileCollideInfo;
}