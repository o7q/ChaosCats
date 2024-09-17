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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            playerSpeed = 12.0f;
        }
        else
        {
            playerSpeed = 6.0f;
        }

        sf::Vector2f vel(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            vel.x = -playerSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            vel.x = playerSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (jumpTimer < playerJumpVelocity * 6)
            {
                vel.y = playerJumpVelocity;
                jumpTimer += playerJumpVelocity;
            }
        }

        if (vel.y < playerGravity)
        {
            vel.y += playerGravityPullSpeed;
        }

        std::vector<Segment<float>> collisionMesh = world->getCollisionMesh();
        bool intersected = false;
        for (Segment<float> segment : collisionMesh)
        {
            if (segmentRect(segment, collisionBox))
            {
                intersected = true;
                break;
            }
        }
        if (intersected)
        {
        }
        else
        {
            // THESE GO TOGETHER
            simulationStep(collisionBox, collisionMesh, dt, vel.x, vel.y, position.x, position.y);
            collisionBox.setPosition(position.x, position.y);
        }

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

    bool onGround = true;
    float playerSpeed = 6.0f;
    float playerGravity = 20.0f;
    float playerGravityPullSpeed = 10.0f;
    float playerJumpVelocity = -40.0f;
    float jumpTimer = 0.0f;
    sf::Vector2f playerVelocity = sf::Vector2f(0, 0);
};