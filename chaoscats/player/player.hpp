#pragma once

#include <SFML/Graphics.hpp>

#include "../math/geometry/geometry_common.hpp"
#include "../rendering/draw.hpp"

class Player
{
public:
    Player(sf::RenderWindow &renderWindow, sf::Texture &playerTexture) : renderWindow(renderWindow), playerTexture(playerTexture)
    {
    }

    void update()
    {
        float speed = 1.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            speed = 3.0f;
        }

        sf::Vector2f vel(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            vel.y -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            vel.y += speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            vel.x -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            vel.x += speed;
        }

        position.x += vel.x;
        position.y += vel.y;

        collisionBox = jmath::Rectangle<float>(jmath::Point<float>(position.x + 5, position.y + 5), jmath::Size<float>(90, 90));
    }

    void draw()
    {
        drawTexture(renderWindow, playerTexture, position, sf::Vector2f(100, -1));
        drawRectangle(renderWindow, collisionBox, sf::Color(50, 255, 50, 200), DrawMode::Outline);
    }

protected:
    sf::RenderWindow &renderWindow;
    sf::Texture &playerTexture;
    jmath::Rectangle<float> collisionBox;

    sf::Vector2f position;
};