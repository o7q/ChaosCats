#include "player.hpp"

void Player::handleMovement()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        playerSpeed = 16.0f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        playerSpeed = 0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
    {
        playerSpeed = 40.0f;
    }
    else
    {
        playerSpeed = 6.0f;
    }

    Particle groundParticle = world->getParticleFromWindowCoords(bottomCollisionPoint.x, bottomCollisionPoint.y + 5);
    Particle subGroundParticle = world->getParticleFromWindowCoords(bottomCollisionPoint.x, bottomCollisionPoint.y + 10);

    switch (groundParticle.playerCollisionType)
    {
    case PlayerCollisionType::None:
        if (velocity.y < 10.0f)
        {
            velocity.y += 1.0f * gameInfo.time.DELTA_TIME;
        }
        break;
    case PlayerCollisionType::Solid:
        if (!jumped)
        {
            velocity.y = 0.0f;
        }
        break;
    }

    bool onGround = subGroundParticle.playerCollisionType == PlayerCollisionType::Solid;

    velocity.x = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && gameInfo.window.IS_FOCUSED)
    {
        velocity.x = -playerSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && gameInfo.window.IS_FOCUSED)
    {
        velocity.x = playerSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameInfo.window.IS_FOCUSED)
    {
        if (onGround && !jumped)
        {
            velocity.y = -15.0f;
            jumped = true;
        }
    }
    else
    {
        jumped = false;
    }
}

void Player::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;

    body_hitbox.setPosition(position.x, position.y + body_hitbox.half_size.y - leftLeg_hitbox.half_size.y);
    head_hitbox.setPosition(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y);

    leftLeg_hitbox.setPosition(position.x - leftLeg_hitbox.half_size.x, position.y + body_hitbox.half_size.y + leftLeg_hitbox.half_size.y);
    rightLeg_hitbox.setPosition(position.x + leftLeg_hitbox.half_size.x, position.y + body_hitbox.half_size.y + leftLeg_hitbox.half_size.y);

    player_collisionBox.setPosition(position.x, position.y);

    bottomCollisionPoint.x = player_collisionBox.getPosition().x;
    bottomCollisionPoint.y = player_collisionBox.getPosition().y + player_collisionBox.half_size.y;

    topCollisionPoint.x = player_collisionBox.getPosition().x;
    topCollisionPoint.y = player_collisionBox.getPosition().y - player_collisionBox.half_size.y;
}