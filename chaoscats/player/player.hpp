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
    }
    virtual ~Player()
    {
        delete weapon;
    }

    void pickupWeapon(sf::Texture &texture)
    {
        weapon = new Shotgun(world, gameInfo, gameAssets);
    }

    const Weapon *getWeapon() const
    {
        return weapon;
    }

    void handleMovement();
    void handleWeapons();
    void handleCollision();

    const std::vector<ProjectileCollideInfo> &getProjectileCollideInfo() const;

    // this function only runs for the local player (client-side)
    void clientUpdate()
    {
        handleMovement();
        handleWeapons();
        handleCollision();

        setPosition(position.x, position.y);
    }

    void serverUpdate()
    {
        handleWeapons();
        setPosition(position.x, position.y);
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
        aimLocation.x = x;
        aimLocation.y = y;
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

        weapon->render(renderWindow, position.x, position.y);
    }

protected:
    const World *world = nullptr;
    GameInfo &gameInfo;
    GameAssets &gameAssets;

protected:
    Point<float> position;
    Point<float> velocity;

protected:
    Weapon *weapon = nullptr;
    std::vector<ProjectileCollideInfo> projectileCollideInfo;
    Point<float> aimLocation;

protected:
    Rect<float> collisionBox;
    Point<float> bottomCollisionPoint;
    Point<float> topCollisionPoint;

    Point<float> playerSize;

    float playerSpeed = 6.0f;
    bool jumped = false;
};