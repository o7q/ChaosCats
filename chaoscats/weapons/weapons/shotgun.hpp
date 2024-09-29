#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../../rendering/draw.hpp"
#include "../weapon.hpp"

class Shotgun : public Weapon
{
public:
    Shotgun(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, World *world, sf::Texture &weaponTexture) : Weapon(renderWindow, windowInfo, world, weaponTexture)
    {
    }
    virtual ~Shotgun() override
    {
    }

    virtual void shoot() override
    {
        if (windowInfo.INPUT_INFO.MOUSE_CLICKED)
        {
            int shellCount = 3;
            float spreadAngle = 15.0f;
            float halfSpreadAngle = spreadAngle / 2.0f;

            float dx = aimX - playerX;
            float dy = aimY - playerY;

            float baseAngle = std::atan2(dy, dx);

            for (int i = 0; i < shellCount; ++i)
            {
                float angleOffset = i * (spreadAngle / (shellCount - 1)) - halfSpreadAngle;
                float angleOffsetDegrees = angleOffset * (PI / 180.0f);
                float newAngle = baseAngle + angleOffsetDegrees;

                float new_dx = std::cos(newAngle);
                float new_dy = std::sin(newAngle);

                Projectile projectile(renderWindow, windowInfo, world, playerX, playerY, new_dx, new_dy, 30.0f);
                projectiles.push_back(projectile);
            }
        }
    }

    virtual void update(float playerX, float playerY, float aimX, float aimY) override
    {
        this->playerX = playerX;
        this->playerY = playerY;
        this->aimX = aimX;
        this->aimY = aimY;

        calculateRotation(playerX, playerY, aimX, aimY);

        for (Projectile &projectile : projectiles)
        {
            projectile.update();
        }
    }

    void render(float x, float y) override
    {
        sf::Vector2i rotateVector(1, 1);
        if (rotation > 90 && rotation < 270)
        {
            rotateVector = sf::Vector2i(1, -1);
        }

        for (Projectile &projectile : projectiles)
        {
            projectile.render();
        }

        drawTexture(renderWindow, weaponTexture, sf::Vector2f(x, y), sf::Vector2f(120, -1), rotation, rotateVector);
    }
};