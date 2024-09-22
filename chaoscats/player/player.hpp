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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            vel.y = -playerSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            vel.y = playerSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            vel.x = -playerSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            vel.x = playerSpeed;
        }

        std::vector<Segment<float>> collisionMesh = world->getCollisionMesh();
        for (Segment<float> segment : collisionMesh)
        {
        }

        collisionBox.incrementPosition(vel.x, vel.y);
        bottomCollisionPoint.x = collisionBox.getPosition().x;
        bottomCollisionPoint.y = collisionBox.getPosition().y + collisionBox.half_size.h;
    }

    void render(WorldCamera *camera)
    {
        // bool collide =
        //     drawRect(renderWindow, collisionBox, sf::Color(255, 0, 255, 255), DrawMode::Outline);
        // int x, y;
        // camera->worldToScreen(position.x, position.y, x, y);
        // drawTexture(renderWindow, playerTexture, sf::Vector2f(x, y), playerSize);

        int worldX = 0;
        int worldY = 0;

        int pixelSize = world->getPixelSize();

        worldX = std::floor((bottomCollisionPoint.x + pixelSize / 2) / static_cast<float>(pixelSize));
        worldY = std::floor((bottomCollisionPoint.y+ pixelSize / 2)  / static_cast<float>(pixelSize));

        Rect<int> rect(worldX * pixelSize, worldY * pixelSize, pixelSize, pixelSize);
        drawRect(renderWindow, rect, sf::Color(50, 200, 255, 100));

        bool collide = false;
        drawSegment(renderWindow, collisionBox.segments[0], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[1], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[2], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawSegment(renderWindow, collisionBox.segments[3], collide ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        drawPoint(renderWindow, bottomCollisionPoint, sf::Color(255, 0, 0, 255));
    }

protected:
    sf::RenderWindow &renderWindow;
    World *world = nullptr;
    sf::Texture &playerTexture;
    sf::Vector2f playerSize;

    Rect<float> collisionBox;
    Point<float> bottomCollisionPoint;

    sf::Vector2f position;

    float playerSpeed = 6.0f;
};