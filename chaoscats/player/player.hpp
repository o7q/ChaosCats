#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../weapons/weapons/shotgun.hpp"

#include "../world/world.hpp"
#include "../world/collision/collision.hpp"

#include "../math/geometry/geometry_common.hpp"
#include "../rendering/draw.hpp"

#include "../net/data/player.hpp"

class Player
{
public:
    Player(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, const World *world, sf::Texture &playerTexture, sf::Vector2f size) : renderWindow(renderWindow),
                                                                                                                                        windowInfo(windowInfo),
                                                                                                                                        world(world),
                                                                                                                                        playerTexture(playerTexture)
    {
        playerSize = imageAspect(playerTexture, size);
        collisionBox = Rect<float>(0, 0, playerSize.x, playerSize.y);
    }
    virtual ~Player()
    {
        delete weapon;
    }

    void pickupWeapon(sf::Texture &texture)
    {
        weapon = new Shotgun(renderWindow, windowInfo, world, texture);
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
    void update()
    {
        handleMovement();
        handleWeapons();
        handleCollision();

        setPosition(playerData.position.x, playerData.position.y);
    }

    void updateFromPlayerData(PlayerData playerData)
    {
        this->playerData = playerData;

        setPosition(playerData.position.x, playerData.position.y);
        setAim(playerData.aim.x, playerData.aim.y);
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

    void setAim(float x, float y)
    {
        playerData.aim.x = x;
        playerData.aim.y = y;
    }

    void setID(int id)
    {
        playerData.id = id;
    }

    const PlayerData &getPlayerData() const
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
        drawPoint(renderWindow, bottomCollisionPoint, sf::Color(255, 0, 0, 255));
        drawPoint(renderWindow, topCollisionPoint, sf::Color(255, 0, 0, 255));

        weapon->render(playerData.position.x, playerData.position.y);
    }

protected:
    sf::RenderWindow &renderWindow;
    WindowInfo &windowInfo;
    const World *world = nullptr;

    sf::Texture &playerTexture;
    sf::Vector2f playerSize;

    Rect<float> collisionBox;
    Point<float> bottomCollisionPoint;
    Point<float> topCollisionPoint;

    PlayerData playerData;
    std::vector<ProjectileCollideInfo> projectileCollideInfo;

    Point<float> velocity;

    float playerSpeed = 6.0f;
    bool jumped = false;

    Weapon *weapon = nullptr;
};