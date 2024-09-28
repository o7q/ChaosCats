#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../object/weapon.hpp"
#include "../object/weapons/shotgun.hpp"

#include "../world/world.hpp"
#include "../world/collision/collision.hpp"

#include "../math/geometry/geometry_common.hpp"
#include "../rendering/draw.hpp"

#include "../net/data/player.hpp"

class Player
{
public:
    Player(sf::RenderWindow &renderWindow, World *world, sf::Texture &playerTexture, sf::Vector2f size) : renderWindow(renderWindow), playerTexture(playerTexture)
    {
        this->world = world;
        playerSize = imageAspect(playerTexture, size);
        collisionBox = Rect<float>(0, 0, playerSize.x, playerSize.y);
    }
    virtual ~Player()
    {
        delete weapon;
    }

    void pickupWeapon(sf::Texture &texture)
    {
        weapon = new Shotgun(texture, renderWindow);
    }

    void update(float dt, WindowInfo &windowInfo)
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

        Particle groundParticle = world->getParticleFromWorldCoords(bottomCollisionPoint.x, bottomCollisionPoint.y + 5);
        Particle subGroundParticle = world->getParticleFromWorldCoords(bottomCollisionPoint.x, bottomCollisionPoint.y + 10);

        switch (groundParticle.playerCollisionType)
        {
        case PlayerCollisionType::None:
            if (velocity.y < 10.0f)
            {
                velocity.y += 1.0f * dt;
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && windowInfo.IS_FOCUSED)
        {
            velocity.x = -playerSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && windowInfo.IS_FOCUSED)
        {
            velocity.x = playerSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && windowInfo.IS_FOCUSED)
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

        std::vector<EdgeData> collisionMesh = world->getCollisionMesh();
        bool collisionHappened = false;
        for (EdgeData edge : collisionMesh)
        {
            bool bottomCollision = handleBottomCollision(playerData.position, velocity, collisionBox, edge, bottomCollisionPoint, dt);
            bool topCollision = handleTopCollision(playerData.position, velocity, collisionBox, edge, topCollisionPoint, dt);

            if (bottomCollision || topCollision)
            {
                collisionHappened = true;
            }
        }

        if (!collisionHappened)
        {
            playerData.position.y += velocity.y * dt;
            playerData.position.x += velocity.x * dt;
        }

        setPosition(playerData.position.x, playerData.position.y);

        sf::Vector2i mouseLoc = sf::Mouse::getPosition(renderWindow);
        weapon->aim(playerData.position.x, playerData.position.y, mouseLoc.x, mouseLoc.y);
    }

    void setPosition(float x, float y)
    {
        playerData.position.x = x;
        playerData.position.y = y;

        collisionBox.setPosition(playerData.position.x, playerData.position.y);

        bottomCollisionPoint.x = collisionBox.getPosition().x;
        bottomCollisionPoint.y = collisionBox.getPosition().y + collisionBox.half_size.h;

        topCollisionPoint.x = collisionBox.getPosition().x;
        topCollisionPoint.y = collisionBox.getPosition().y - collisionBox.half_size.h;
    }

    void setID(int id)
    {
        playerData.id = id;
    }

    PlayerData getPlayerData()
    {
        return playerData;
    }

    void render(WorldCamera *camera)
    {
        bool collide = false;
        drawSegment(renderWindow, collisionBox.segments[0], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[1], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[2], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[3], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        // drawTexture(renderWindow, playerTexture, sf::Vector2f(position.x, position.y), sf::Vector)
        drawPoint(renderWindow, bottomCollisionPoint, sf::Color(255, 0, 0, 255));
        drawPoint(renderWindow, topCollisionPoint, sf::Color(255, 0, 0, 255));

        weapon->render(playerData.position.x, playerData.position.y);
    }

protected:
    sf::RenderWindow &renderWindow;
    World *world = nullptr;
    sf::Texture &playerTexture;
    sf::Vector2f playerSize;

    Rect<float> collisionBox;
    Point<float> bottomCollisionPoint;
    Point<float> topCollisionPoint;

    PlayerData playerData;
    Point<float> velocity;

    float playerSpeed = 6.0f;
    bool jumped = false;

    Weapon *weapon = nullptr;
};