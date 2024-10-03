#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../../rendering/draw.hpp"
#include "../weapon.hpp"

class Shotgun : public Weapon
{
public:
    Shotgun(const World *world, GameInfo &gameInfo, GameAssets &assets) : Weapon(world, gameInfo, assets)
    {
        sf::Vector2f aspect = imageAspect(gameAssets.textures.WEAPON_SHOTGUN_TEXTURE, sf::Vector2f(100, -1));
        weaponSize.x = aspect.x;
        weaponSize.y = aspect.y;
    }
    virtual ~Shotgun() override
    {
    }

    virtual Point<float> shoot(float playerX, float playerY, float aimX, float aimY) override
    {
        int shellCount = 10;
        float spreadAngle = 15.0f;
        float halfSpreadAngle = spreadAngle / 2.0f;

        float dx = aimX - playerX;
        float dy = aimY - playerY;

        float baseAngle = std::atan2(dy, dx);

        ProjectileSettings projectileSettings;
        projectileSettings.damageRadius = 3;
        projectileSettings.playerDamage = 3;
        projectileSettings.speed = 100.0f;

        for (int i = 0; i < shellCount; ++i)
        {
            float angleOffset = i * (spreadAngle / (shellCount - 1)) - halfSpreadAngle;
            float angleOffsetDegrees = angleOffset * (PI / 180.0f);
            float newAngle = baseAngle + angleOffsetDegrees;

            float new_dx = std::cos(newAngle);
            float new_dy = std::sin(newAngle);

            Projectile projectile(world, projectileSettings, Point<float>(playerX, playerY), Point<float>(new_dx, new_dy));
            projectiles.push_back(projectile);
        }

        float distance = std::sqrt(dx * dx + dy * dy);
        float dirX = dx / distance;
        float dirY = dy / distance;

        return Point<float>(-dirX * 30, -dirY * 10);
    }

    virtual std::vector<ProjectileCollideInfo> update(float playerX, float playerY, float aimX, float aimY) override
    {
        std::vector<ProjectileCollideInfo> hitPoints;

        for (Projectile &projectile : projectiles)
        {
            ProjectileCollideInfo projectileCollideInfo = projectile.update(gameInfo.time.DELTA_TIME);

            if (projectile.hasHit())
            {
                hitPoints.push_back(projectileCollideInfo);
            }
        }

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](const Projectile &projectile)
                           {
                               return projectile.hasHit() || projectile.outOfBounds();
                           }),
            projectiles.end());

        return hitPoints;
    }

    void render(sf::RenderWindow &renderWindow, float x, float y, float aimX, float aimY) override
    {
        float rotation = calculateRotation(x, y, aimX, aimY);

        sf::Vector2i rotateVector(1, 1);
        if (rotation > 90 && rotation < 270)
        {
            rotateVector = sf::Vector2i(1, -1);
        }

        for (Projectile &projectile : projectiles)
        {
            projectile.render(renderWindow);
        }

        drawTexture(renderWindow, gameAssets.textures.WEAPON_SHOTGUN_TEXTURE, sf::Vector2f(x, y), sf::Vector2f(weaponSize.x, weaponSize.y), rotation, rotateVector);
    }
};