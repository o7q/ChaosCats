#pragma once

#include <SFML/Graphics.hpp>

#include "object.hpp"
#include "player/player.hpp"

class Weapon : public Object
{
public:
    Weapon(sf::Texture &weaponTexture, sf::RenderWindow &renderWindow) : weaponTexture(weaponTexture), renderWindow(renderWindow)
    {

    }
    virtual ~Weapon()
    {
        std::cout << "delete weapon" << std::endl;
    }

    virtual void shoot()
    {
        std::cout << "fire weapon" << std::endl;
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