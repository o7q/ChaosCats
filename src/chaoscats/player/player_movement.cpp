#include "player.hpp"

#include <cmath>

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

    // calculate damping value, used to slow player for certain scenarios
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
    //

    // apply gravity to the player
    switch (groundParticle.playerCollisionType)
    {
    case PlayerCollisionType::None:
    case PlayerCollisionType::Liquid:
    {
        // the maxiumum downwards velocity before gravity is stopped being applied to the player
        // if velocity.y > 0.0f, the player is moving down, because coordinates originate from the top-left of the screen
        if (velocity.y < 10.0f / damping)
        {
            // add 1.0f to velocity, divide it by the damping value to make the player fall slower
            // and multiply it by delta time of course :3
            velocity.y += (1.0f / damping) * gameState.time.DELTA_TIME;
        }
    }
    break;
    default:
        break;
    }
    //

    // determine if the player is within a close range of the ground at foot-level
    // used for determining if the player can jump
    bool standingOnGround = false;
    for (int i = -2; i < 10; ++i)
    {
        Particle groundTestParticle = world->getParticleFromWindowCoords(collisionSegment.p2.x, collisionSegment.p2.y + i);
        if (groundTestParticle.playerCollisionType == PlayerCollisionType::Solid)
        {
            standingOnGround = true;
            break;
        }
    }
    //

    // apply x air velocity, used for when the player is knocked back in some way
    if (enableXAirVelocity)
    {
        // slow down player
        velocity.x *= 0.95f / damping;

        // if the player is moving less than this value, stop the player to prevent the player from infinitely getting slower
        if (std::fabs(velocity.x) < 1.0f)
        {
            velocity.x = 0;
            enableXAirVelocity = false;
        }
    }
    else
    {
        // if x air velocity is disabled, just stop the player instantly for smooth and snappy feedback
        velocity.x = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && gameState.window.IS_FOCUSED)
    {
        // move the player left
        if (!enableXAirVelocity)
        {
            velocity.x = -speed / damping;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && gameState.window.IS_FOCUSED)
    {
        // move the player right
        if (!enableXAirVelocity)
        {
            velocity.x = speed / damping;
        }
    }

    // calculate coyote time
    if (standingOnGround)
    {
        coyoteTimeCounter = 10.0f;
    }
    // deplete coyote time by delta time if its greater than 0
    else if (coyoteTimeCounter > 0.0f)
    {
        coyoteTimeCounter -= gameState.time.DELTA_TIME;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameState.window.IS_FOCUSED)
    {
        // let the player jump if they are standing on the ground or coyote time lets them, and haven't jumped
        // OR
        // if the player is swimming in liquid
        if (
            ((standingOnGround || coyoteTimeCounter > 0.0f) && !jumped) ||
            bodyParticle.playerCollisionType == PlayerCollisionType::Liquid)
        {
            // overwrite the player's velocity to -15.0f divided by the damping value
            velocity.y = -15.0f / damping;
            // jumped prevents the player from holding space and jumping, but it allows the player to buffer future jumps
            jumped = true;
            // disable coyote time to prevent the player from double jumping
            coyoteTimeCounter = 0.0f;
        }
        //
    }
    else
    {
        jumped = false;
    }
}

void Player::updateChildPositions()
{
    body_boundingBox.setPosition(position.x, position.y + body_boundingBox.half_size.y - leftLeg_boundingBox.half_size.y);
    head_boundingBox.setPosition(position.x, position.y - head_boundingBox.half_size.y - leftLeg_boundingBox.half_size.y);

    leftLeg_boundingBox.setPosition(position.x - leftLeg_boundingBox.half_size.x, position.y + body_boundingBox.half_size.y + leftLeg_boundingBox.half_size.y);
    rightLeg_boundingBox.setPosition(position.x + leftLeg_boundingBox.half_size.x, position.y + body_boundingBox.half_size.y + leftLeg_boundingBox.half_size.y);

    boundingBox.setPosition(position.x, position.y);

    Vector<float> bottomCollisionPoint(
        boundingBox.getPosition().x,
        boundingBox.getPosition().y + boundingBox.half_size.y);

    Vector<float> topCollisionPoint(
        boundingBox.getPosition().x,
        boundingBox.getPosition().y - boundingBox.half_size.y);

    collisionSegment.p1 = topCollisionPoint;
    collisionSegment.p2 = bottomCollisionPoint;
}