#pragma once

#include <iostream>
#include <utility>

#include <SFML/Graphics.hpp>

#include "../../rendering/draw.hpp"
#include "../../math/trig.hpp"
#include "../weapon.hpp"

class Shotgun : public Weapon
{
public:
    Shotgun(const World *world,
            GameInfo &gameInfo,
            GameAssets &gameAssets) : Weapon(world,
                                             gameInfo,
                                             gameAssets)
    {
        int scale = 4;
        sf::Vector2u textureSize = gameAssets.textures.WEAPON_SHOTGUN_TEXTURE.getSize();

        sf::Vector2f aspect = imageAspect(gameAssets.textures.WEAPON_SHOTGUN_TEXTURE, sf::Vector2f(textureSize.x * scale, -1));

        sf::Image gunPointsImage = gameAssets.textures.WEAPON_SHOTGUN_POINTS_TEXTURE.copyToImage();

        for (unsigned int y = 0; y < gunPointsImage.getSize().y; ++y)
        {
            for (unsigned int x = 0; x < gunPointsImage.getSize().x; ++x)
            {
                sf::Color pixColor = gunPointsImage.getPixel(x, y);
                if (pixColor == sf::Color(255, 0, 0, 255))
                {
                    muzzlePosition = Vector<int>(x, y);
                }
                else if (pixColor == sf::Color(0, 255, 0, 255))
                {
                    handMountPosition1 = Vector<int>(x, y);
                }
                else if (pixColor == sf::Color(0, 0, 255, 255))
                {
                    handMountPosition2 = Vector<int>(x, y);
                }
            }
        }

        weaponSize.x = aspect.x;
        weaponSize.y = aspect.y;

        weaponPositionOffset.x = 20;
        weaponPositionOffset.y = 5;
    }
    virtual ~Shotgun() override
    {
    }

    Vector<float> shoot(float playerX, float playerY, float aimX, float aimY) override
    {
        int shellCount = 10;
        float spreadAngle = 15.0f;
        float halfSpreadAngle = spreadAngle / 2.0f;

        Vector<float> mp = rotatePoint(Vector<float>(
                                           muzzlePosition.x * scale - weaponSize.x / 2,
                                           muzzlePosition.y * scale - weaponSize.y / 2),
                                       rotation_radians);

        float x_new = mp.x + playerX;
        float y_new = mp.y + playerY;

        float dx = aimX - x_new;
        float dy = aimY - y_new;

        float baseAngle = std::atan2(dy, dx);

        ProjectileSettings projectileSettings;
        projectileSettings.damageRadius = 3;
        projectileSettings.playerDamage = 3;
        projectileSettings.speed = 90.0f;

        for (int i = 0; i < shellCount; ++i)
        {
            float angleOffset = i * (spreadAngle / (shellCount - 1)) - halfSpreadAngle;
            float angleOffsetDegrees = angleOffset * (PI / 180.0f);
            float newAngle = baseAngle + angleOffsetDegrees;

            float new_dx = std::cos(newAngle);
            float new_dy = std::sin(newAngle);

            Projectile projectile(world, projectileSettings, Vector<float>(x_new, y_new), Vector<float>(new_dx, new_dy));
            projectiles.push_back(projectile);
        }

        float distance = std::sqrt(dx * dx + dy * dy);
        float dirX = dx / distance;
        float dirY = dy / distance;

        return Vector<float>(-dirX * 30, -dirY * 10);
    }

    virtual void update(float playerX, float playerY, float aimX, float aimY) override
    {
        for (Projectile &projectile : projectiles)
        {
            projectile.update(gameInfo.time.DELTA_TIME);
        }

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                           [](const Projectile &projectile)
                           {
                               return projectile.hasHit() || projectile.outOfBounds();
                           }),
            projectiles.end());

        rotation_radians = radiansFromPoints(playerX, playerY, aimX, aimY);
        rotation_degrees = degreesFromPoints(playerX, playerY, aimX, aimY);
    }

    void render(sf::RenderWindow &renderWindow, float x, float y, float aimX, float aimY) override
    {
        weaponPositionOffset.x = std::fabs(weaponPositionOffset.x);
        weaponPositionOffset.y = std::fabs(weaponPositionOffset.y);

        sf::Vector2i rotateVector(1, 1);
        if (rotation_degrees > 90 && rotation_degrees < 270)
        {
            rotateVector = sf::Vector2i(1, -1);
            weaponPositionOffset.x *= -1;
        }

        for (Projectile &projectile : projectiles)
        {
            projectile.render(renderWindow);
        }

        drawTexture(renderWindow,
                    gameAssets.textures.WEAPON_SHOTGUN_TEXTURE,
                    sf::Vector2f(x + weaponPositionOffset.x,
                                 y + weaponPositionOffset.y),
                    sf::Vector2f(weaponSize.x, weaponSize.y),
                    rotation_degrees,
                    rotateVector);
    }
};