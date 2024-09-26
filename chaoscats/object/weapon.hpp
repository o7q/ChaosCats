#pragma once

#include <SFML/Graphics.hpp>

#include "player/player.hpp"

class Weapon
{
public:
    Weapon(sf::Texture &weaponTexture, sf::RenderWindow &renderWindow) : weaponTexture(weaponTexture), renderWindow(renderWindow)
    {

    }
    virtual ~Weapon()
    {

    }

    virtual void shoot()
    {

    }

    virtual void aim(float playerX, float playerY, float aimX, float aimY)
    {
    }

    virtual void render(float x, float y)
    {

    }

protected:
    sf::Texture &weaponTexture;
    sf::RenderWindow &renderWindow;

    float rotation = 0;
};