#include "player.hpp"

void Player::setHealth(float health)
{
    this->health = health;
}

float Player::getHealth() const
{
    return health;
}

void Player::kill()
{
    health -= 10000.0f;
}

bool Player::isDead() const
{
    return health <= 0.0f;
}

PlayerDamageModel &Player::getDamageModel()
{
    return damageModel;
}

void Player::damage(ProjectileCollideInfo collideInfo)
{
    switch (collideInfo.collideType)
    {
    case CollideType::PlayerHead:
    {
        int x = (collideInfo.x - head_boundingBox.getPosition().x + head_boundingBox.half_size.x) / scale;
        int y = (collideInfo.y - head_boundingBox.getPosition().y + head_boundingBox.half_size.y) / scale;
        damageModel.head->damage(x, y, collideInfo.projectileSettings.damageRadius);

        health -= collideInfo.projectileSettings.playerDamage;
    }
    break;
    case CollideType::PlayerBody:
    {
        int x = (collideInfo.x - body_boundingBox.getPosition().x + body_boundingBox.half_size.x) / scale;
        int y = (collideInfo.y - body_boundingBox.getPosition().y + body_boundingBox.half_size.y) / scale;
        damageModel.body->damage(x, y, collideInfo.projectileSettings.damageRadius);

        health -= collideInfo.projectileSettings.playerDamage;
    }
    break;
    default:
        break;
    }

    std::cout << "health: " << health << std::endl;
}