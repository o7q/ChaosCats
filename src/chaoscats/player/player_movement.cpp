#include "player.hpp"

void Player::handleMovement()
{
    float speed = 6.0f;
    // increase player speed when the sprint key is held
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        speed = 16.0f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
    {
        speed = 40.0f;
    }

    // ground particle is the particle directly below the player's feet
    // bodyParticle is the particle located near the center of the player's body
    Particle groundParticle = world->getParticleFromWindowCoords(collisionSegment.p2.x, collisionSegment.p2.y + 1);
    Particle bodyParticle = world->getParticleFromWindowCoords(collisionSegment.p2.x, collisionSegment.p2.y - 15);

    float damping = 1.0f;

    switch (bodyParticle.playerMovementDampingType)
    {
    case PlayerMovementDampingType::Water:
    {
        damping = 2.0f;
    }
    break;
    case PlayerMovementDampingType::Solid:
    {
        damping = 100.0f;
    }
    break;
    default:
        break;
    }

    switch (groundParticle.playerCollisionType)
    {
    case PlayerCollisionType::None:
    case PlayerCollisionType::Liquid:
    {
        // the maxiumum downwards velocity before gravity is stopped being applied to the player
        if (velocity.y < 10.0f / damping)
        {
            velocity.y += (1.0f / damping) * gameInfo.time.DELTA_TIME;
        }
    }
    break;
    default:
        break;
    }

    bool bottomTouchingSubstance = false;
    for (int i = -5; i < 10; ++i)
    {
        Particle groundTestParticle = world->getParticleFromWindowCoords(collisionSegment.p2.x, collisionSegment.p2.y + i);
        if (groundTestParticle.playerCollisionType == PlayerCollisionType::Solid ||
            groundTestParticle.playerCollisionType == PlayerCollisionType::Liquid)
        {
            bottomTouchingSubstance = true;
            break;
        }
    }

    velocity.x = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && gameInfo.window.IS_FOCUSED)
    {
        velocity.x = -speed / damping;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && gameInfo.window.IS_FOCUSED)
    {
        velocity.x = speed / damping;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameInfo.window.IS_FOCUSED)
    {
        if ((bottomTouchingSubstance && !jumped) || bodyParticle.playerCollisionType == PlayerCollisionType::Liquid)
        {
            velocity.y = -15.0f / damping;
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

    Vector<float> bottomCollisionPoint(
        player_collisionBox.getPosition().x,
        player_collisionBox.getPosition().y + player_collisionBox.half_size.y);

    Vector<float> topCollisionPoint(
        player_collisionBox.getPosition().x,
        player_collisionBox.getPosition().y - player_collisionBox.half_size.y);

    collisionSegment.p1 = topCollisionPoint;
    collisionSegment.p2 = bottomCollisionPoint;
}