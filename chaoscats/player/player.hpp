#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../weapons/weapons/shotgun.hpp"

#include "../world/world.hpp"
#include "../world/collision/collision.hpp"

#include "../math/geometry/geometry_common.hpp"
#include "../rendering/draw.hpp"

class Player
{
public:
    Player(const World *world, GameInfo &gameInfo, GameAssets &gameAssets) : world(world),
                                                                             gameInfo(gameInfo),
                                                                             gameAssets(gameAssets)
    {
        playerSize = Point<float>(100, 100);
        collisionBox = Rect<float>(0, 0, playerSize.x, playerSize.y);
        weapon = new Shotgun(world, gameInfo, gameAssets);
    }
    virtual ~Player()
    {
        delete weapon;
    }

    void pickupWeapon()
    {
    }

    const Weapon *getWeapon() const
    {
        return weapon;
    }

    void handleMovement();
    void handleCollision();

    // this function only runs for the local player (client-side)
    void clientUpdate()
    {
        handleMovement();
        handleCollision();

        setPosition(position.x, position.y);
        if (gameInfo.window.IS_FOCUSED)
        {
            setAim(gameInfo.input.MOUSE_POS.x, gameInfo.input.MOUSE_POS.y);
        }

        weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    void visualClientUpdate()
    {
        weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    void serverUpdate()
    {
        projectileCollideInfo.clear();
        projectileCollideInfo = projectileCollideInfo = weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    const std::vector<ProjectileCollideInfo> &getProjectileCollideInfo() const
    {
        return projectileCollideInfo;
    }

    void shoot()
    {
        weapon->shoot(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    void setPosition(float x, float y)
    {
        position.x = x;
        position.y = y;

        collisionBox.setPosition(position.x, position.y);

        bottomCollisionPoint.x = collisionBox.getPosition().x;
        bottomCollisionPoint.y = collisionBox.getPosition().y + collisionBox.half_size.h;

        topCollisionPoint.x = collisionBox.getPosition().x;
        topCollisionPoint.y = collisionBox.getPosition().y - collisionBox.half_size.h;
    }

    Point<float> getPosition() const
    {
        return position;
    }

    void setAim(float x, float y)
    {
        aimPosition.x = x;
        aimPosition.y = y;
    }

    Point<float> getAim() const
    {
        return aimPosition;
    }

    void render(sf::RenderWindow &renderWindow)
    {
        bool collide = false;
        drawSegment(renderWindow, collisionBox.segments[0], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[1], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[2], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[3], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawPoint(renderWindow, bottomCollisionPoint, sf::Color(255, 0, 0, 255));
        drawPoint(renderWindow, topCollisionPoint, sf::Color(255, 0, 0, 255));

        weapon->render(renderWindow, position.x, position.y, aimPosition.x, aimPosition.y);
    }

protected:
    const World *world = nullptr;
    GameInfo &gameInfo;
    GameAssets &gameAssets;

protected:
    Point<float> position = Point<float>(0, 0);
    Point<float> velocity = Point<float>(0, 0);

protected:
    Weapon *weapon = nullptr;
    std::vector<ProjectileCollideInfo> projectileCollideInfo;
    Point<float> aimPosition = Point<float>(0, 0);

protected:
    Rect<float> collisionBox;
    Point<float> bottomCollisionPoint;
    Point<float> topCollisionPoint;

    Point<float> playerSize;

    float playerSpeed = 6.0f;
    bool jumped = false;
};