#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "projectile.hpp"

#include "../rendering/draw.hpp"
#include "../game_info/game_info.hpp"

#include "../math/constants.hpp"

#include "../world/world.hpp"

#include "../game_assets/game_assets.hpp"

class Weapon
{
public:
    Weapon(const World *world, GameInfo& gameInfo, GameAssets &gameAssets) : world(world), gameInfo(gameInfo), gameAssets(gameAssets)
    {
    }
    virtual ~Weapon()
    {
    }

    virtual Point<float> shoot(float playerX, float playerY, float aimX, float aimY)
    {
        return Point<float>(0, 0);
    }

    virtual std::vector<ProjectileCollideInfo> update(float playerX, float playerY, float aimX, float aimY)
    {
        return std::vector<ProjectileCollideInfo>();
    }

    virtual void render(sf::RenderWindow &renderWindow, float playerX, float playerY, float aimX, float aimY)
    {
    }

protected:
    float calculateRotation(float startX, float startY, float endX, float endY)
    {
        float dx = endX - startX;
        float dy = endY - startY;
        float radians = std::atan2(dy, dx);
        float degrees = (180.0f / PI) * radians;
        float degrees360 = std::fmod(degrees + 360.0f, 360.0f);

        return degrees360;
    }

protected:
    const World *world;
    GameInfo& gameInfo;
    GameAssets& gameAssets;

    std::vector<Projectile> projectiles;

    Point<float> muzzlePosition;
    Point<float> handMountPosition1;
    Point<float> handMountPosition2;

    Point<float> weaponSize;

    float playerX = 0;
    float playerY = 0;
    float aimX = 0;
    float aimY = 0;
};