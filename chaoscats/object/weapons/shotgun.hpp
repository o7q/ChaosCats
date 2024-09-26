#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../../rendering/draw.hpp"
#include "../weapon.hpp"
#include "../player/player.hpp"

#define PI 3.1415926535

class Shotgun : public Weapon
{
public:
    Shotgun(sf::Texture &weaponTexture, sf::RenderWindow &renderWindow) : Weapon(weaponTexture, renderWindow)
    {
    }
    virtual ~Shotgun()
    {

    }

    void shoot() override
    {

    }

    void aim(float playerX, float playerY, float aimX, float aimY) override
    {
        float dx = aimX - playerX;
        float dy = aimY - playerY;
        float radians = std::atan2(dy, dx);
        float degrees = (180.0f / PI) * radians;
        float degrees360 = std::fmod(degrees + 360.0f, 360.0f);

        rotation = degrees360;
    }

    void render(float x, float y) override
    {
        drawTexture(renderWindow, weaponTexture, sf::Vector2f(x, y), sf::Vector2f(40, -1), rotation, true);
    }
};