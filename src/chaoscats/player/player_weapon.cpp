#include "player.hpp"

Weapon *Player::getActiveWeapon()
{
    return weapon;
}

void Player::shoot(Camera *camera)
{
    Vector<float> kickbackVelocity = weapon->shoot(position.x, position.y, aimPosition.x, aimPosition.y);
    velocity = kickbackVelocity;
    enableXAirVelocity = true;
    if (camera != nullptr)
    {
        camera->shake(20.0f, 10.0f);
    }
}