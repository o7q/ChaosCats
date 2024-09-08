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
    Player(sf::RenderWindow &renderWindow, sf::Texture &playerTexture) : renderWindow(renderWindow), playerTexture(playerTexture)
    {
    }

    void update(std::vector<jmath::Segment<float>> collisionMesh)
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

        jmath::SR_OverlapData overlapData;
        float slope = 0.0f;
        for (jmath::Segment<float> seg : collisionMesh)
        {
            overlapData = jmath::segmentRectangle(seg, collisionBox);

            if (overlapData.overlapping)
            {
                slope = -(seg.p2.y - seg.p1.y) / (seg.p2.x - seg.p1.x);

                jmath::Point<float> segPoint((seg.p1.x + seg.p2.x) / 2, (seg.p1.y + seg.p2.y) / 2);

                jmath::Point<float> d(segPoint.x - position.x, segPoint.y - position.y);

                // static resolving math derived from: https://youtu.be/7Ik2vowGcU0?t=2140

                float s = std::sqrt(d.x * d.x + d.y * d.y);

                // float distance = jmath::distancePointToSegment(jmath::Point<float>(position.x + collisionBox.size.w / 2, position.y + collisionBox.size.h / 2), seg.p1, seg.p2);

                float distance = minDistanceFromRectangleToSegment(collisionBox, seg);

                float halfWidth = collisionBox.size.w / 2;
                float halfHeight = collisionBox.size.h / 2;
                float diagonal_dist = std::sqrt(halfWidth * halfWidth + halfHeight * halfHeight);

                float over = distance - diagonal_dist + 63;

                std::cout << over << std::endl;

                position.x -= over * d.x / s;
                position.y -= over * d.y / s;

                break;
            }
        }

        slope = std::clamp(slope, -100.0f, 100.0f);
        bool largeSlope = (slope > 10 || slope < -10);

        // switch (collisionSide)
        // {
        // case jmath::CollisionSide::Left:
        //     std::cout << "left" << std::endl;
        //     position.x += vel.x * (largeSlope ? 0 : 1) + 1;
        //     position.y += (1 + vel.y) * (largeSlope ? 0 : slope);
        //     break;
        // case jmath::CollisionSide::Top:
        //     std::cout << "top" << std::endl;
        //     position.x += vel.x * (largeSlope ? 0 : 1);
        //     position.y += vel.y * (largeSlope ? 0 : -slope) + 1;
        //     break;
        // case jmath::CollisionSide::Right:
        //     std::cout << "right" << std::endl;
        //     position.x += vel.x * (largeSlope ? 0 : 1) - 1;
        //     position.y += (1 + vel.y) * (largeSlope ? 0 : -slope);
        //     break;
        // case jmath::CollisionSide::Bottom:
        //     std::cout << "bottom" << std::endl;
        //     position.x += vel.x * (largeSlope ? 0 : 1);
        //     position.y += vel.y * (largeSlope ? 0 : -slope) - 1;
        //     break;
        // case jmath::CollisionSide::None:
        //     position.x += vel.x;
        //     position.y += vel.y;
        //     break;
        // }

        position.x += vel.x;
        position.y += vel.y;

        collisionBox = jmath::Rectangle<float>(jmath::Point<float>(position.x + 5, position.y + 5), jmath::Size<float>(90, 90));
        drawRectangle(renderWindow, collisionBox, overlapData.overlapping ? sf::Color(255, 50, 50, 200) : sf::Color(50, 255, 50, 200), DrawMode::Outline);
        jmath::Rectangle<float> rect(jmath::Point<float>(position.x + collisionBox.size.w / 2, position.y + collisionBox.size.h / 2), jmath::Size<float>(20, 20));
        drawRectangle(renderWindow, rect, sf::Color(0, 0, 255, 255));
    }

    void draw()
    {
        drawTexture(renderWindow, playerTexture, position, sf::Vector2f(100, -1));
    }

protected:
    sf::RenderWindow &renderWindow;
    sf::Texture &playerTexture;
    jmath::Rectangle<float> collisionBox;

    sf::Vector2f position;
};