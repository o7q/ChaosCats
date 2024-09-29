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
    Weapon(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, World* world, sf::Texture &weaponTexture) : renderWindow(renderWindow), windowInfo(windowInfo), world(world), weaponTexture(weaponTexture)
    {
    }
    virtual ~Weapon()
    {
    }

    virtual void shoot()
    {
    }

    virtual void update(float aimX, float aimY, float playerX, float playerY)
    {
    }

    virtual void render(float x, float y)
    {
    }

protected:
    void calculateRotation(float startX, float startY, float endX, float endY)
    {
        float dx = endX - startX;
        float dy = endY - startY;
        float radians = std::atan2(dy, dx);
        float degrees = (180.0f / PI) * radians;
        float degrees360 = std::fmod(degrees + 360.0f, 360.0f);

        rotation = degrees360;
    }

protected:
    sf::RenderWindow &renderWindow;
    WindowInfo &windowInfo;
    World* world;
    sf::Texture &weaponTexture;

    std::vector<Projectile> projectiles;

    float rotation = 0;

    float playerX = 0;
    float playerY = 0;
    float aimX = 0;
    float aimY = 0;
};