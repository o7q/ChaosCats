#pragma once

#include <iostream>
#include <cmath>
#include <utility>

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
    Weapon(const World *world, GameInfo &gameInfo, GameAssets &gameAssets) : world(world), gameInfo(gameInfo), gameAssets(gameAssets)
    {
    }
    virtual ~Weapon()
    {
    }

    virtual Vector<float> shoot(float playerX, float playerY, float aimX, float aimY)
    {
        return Vector<float>(0, 0);
    }

    virtual std::vector<ProjectileCollideInfo> update(float playerX, float playerY, float aimX, float aimY)
    {
        return std::vector<ProjectileCollideInfo>();
    }

    virtual void render(sf::RenderWindow &renderWindow, float playerX, float playerY, float aimX, float aimY)
    {
    }

    virtual std::pair<Vector<float>, Vector<float>> getHandPlacementPoints(float x, float y)
    {
        Vector<float> handPosition1 = rotatePoint(Vector<float>(
                                                     handMountPosition1.x * scale - weaponSize.x / 2,
                                                     handMountPosition1.y * scale - weaponSize.y / 2),
                                                 rotation_radians);
        Vector<float> handPosition2 = rotatePoint(Vector<float>(
                                                     handMountPosition2.x * scale - weaponSize.x / 2,
                                                     handMountPosition2.y * scale - weaponSize.y / 2),
                                                 rotation_radians);

        handPosition1.x += x + weaponPositionOffset.x;
        handPosition1.y += y + weaponPositionOffset.y;
        handPosition2.x += x + weaponPositionOffset.x;
        handPosition2.y += y + weaponPositionOffset.y;

        return std::pair<Vector<float>, Vector<float>>(handPosition1, handPosition2);
    }

protected:
    const World *world;
    GameInfo &gameInfo;
    GameAssets &gameAssets;

    std::vector<Projectile> projectiles;

    Vector<int> handMountPosition1;
    Vector<int> handMountPosition2;
    Vector<int> muzzlePosition;

    const float scale = 4.0f;
    Vector<float> weaponSize;
    Vector<float> weaponPositionOffset;

    float rotation_radians = 0.0f;
    float rotation_degrees = 0.0f;

    float playerX = 0;
    float playerY = 0;
    float aimX = 0;
    float aimY = 0;
};