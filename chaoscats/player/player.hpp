#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../math/geometry/geometry_common.hpp"
#include "../rendering/draw.hpp"

#include "../world/world.hpp"

class Player
{
public:
    Player(sf::RenderWindow &renderWindow, World *world, sf::Texture &playerTexture, sf::Vector2f size) : renderWindow(renderWindow), playerTexture(playerTexture)
    {
        this->world = world;
        playerSize = imageAspect(playerTexture, size);
        collisionBox = Rect<float>(0, 0, playerSize.x, playerSize.y);
    }

    void update(float dt)
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

        // THESE GO TOGETHER
        simulationStep(collisionBox, world->getCollisionMesh(), dt, vel.x, vel.y, position.x, position.y);
        collisionBox.setPosition(position.x, position.y);

        // position.x += vel.x;
        // position.y += vel.y;

        bool collide = false;
        drawSegment(renderWindow, collisionBox.segments[0], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[1], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[2], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[3], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        // drawRect(renderWindow, collisionBox, sf::Color(255, 255, 255, 255), DrawMode::Outline);
    }

    void render(WorldCamera *camera)
    {
        // bool collide =
        //     drawRect(renderWindow, collisionBox, sf::Color(255, 0, 255, 255), DrawMode::Outline);
        // int x, y;
        // camera->worldToScreen(position.x, position.y, x, y);
        // drawTexture(renderWindow, playerTexture, sf::Vector2f(x, y), playerSize);
    }

protected:
    sf::RenderWindow &renderWindow;
    World *world = nullptr;
    sf::Texture &playerTexture;
    sf::Vector2f playerSize;

    Rect<float> collisionBox;

    sf::Vector2f position;
};