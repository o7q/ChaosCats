#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "projectile.hpp"

#include "../rendering/draw.hpp"
#include "../window/window_info.hpp"

#include "../math/constants.hpp"

#include "../world/world.hpp"

class Weapon
{
public:
    Weapon(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, const World *world, sf::Texture &weaponTexture) : renderWindow(renderWindow), windowInfo(windowInfo), world(world), weaponTexture(weaponTexture)
    {
    }
    virtual ~Weapon()
    {
    }

    virtual Point<float> shoot()
    {
        return Point<float>(0, 0);
    }

    virtual std::vector<ProjectileCollideInfo> update(float playerX, float playerY, float aimX, float aimY)
    {
        return std::vector<ProjectileCollideInfo>();
    }

    virtual void render(float x, float y)
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
    sf::RenderWindow &renderWindow;
    WindowInfo &windowInfo;
    const World *world;
    sf::Texture &weaponTexture;

    std::vector<Projectile> projectiles;

    float playerX = 0;
    float playerY = 0;
    float aimX = 0;
    float aimY = 0;
};