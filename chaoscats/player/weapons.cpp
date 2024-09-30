#include "player.hpp"

void Player::handleWeapons()
{
    sf::Vector2i mouseLocation = sf::Mouse::getPosition(renderWindow);

    projectileCollideInfo.clear();
    projectileCollideInfo = weapon->update(playerData.position.x, playerData.position.y, mouseLocation.x, mouseLocation.y);

    Point<float> newVel = weapon->shoot();

    velocity.x += newVel.x;
    velocity.y += newVel.y;
}

const std::vector<ProjectileCollideInfo>& Player::getProjectileCollideInfo() const
{
    return projectileCollideInfo;
}