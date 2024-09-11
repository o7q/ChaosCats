#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>

#include "../math/geometry/geometry_common.hpp"
#include "../rendering/draw.hpp"
#include "../math/collision/collision.hpp"

class Player
{
public:
    Player(sf::RenderWindow &renderWindow, sf::Texture &playerTexture, sf::Vector2f size) : renderWindow(renderWindow), playerTexture(playerTexture)
    {
        playerSize = imageAspect(playerTexture, size);

        collisionBox = Rect<float>(Point2D<float>(0, 0), Size2D<float>(playerSize.x, playerSize.y));
    }

    void update(std::vector<Segment<float>> worldSegments, WindowInfo windowInfo)
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

        Rect<float> rect(Point2D<float>(position.x + collisionBox.size.w / 2, position.y + collisionBox.size.h / 2), Size2D<float>(20, 20));
        drawRect(renderWindow, rect, sf::Color(0, 0, 255, 255));
    }

    void draw()
    {
        drawTexture(renderWindow, playerTexture, position, playerSize);
    }

protected:
    sf::RenderWindow &renderWindow;
    sf::Texture &playerTexture;
    sf::Vector2f playerSize;

    Rect<float> collisionBox;

    sf::Vector2f position;
};