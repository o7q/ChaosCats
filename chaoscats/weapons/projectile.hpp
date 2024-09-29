#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../rendering/draw.hpp"
#include "../window/window_info.hpp"

#include "../math/constants.hpp"

#include "../world/world.hpp"
#include "../world/collision/collision.hpp"

class Projectile
{
public:
    Projectile(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, World *world, float startX, float startY, float slopeX, float slopeY, float speed) : renderWindow(renderWindow),
                                                                                                                                                            world(world),
                                                                                                                                                            windowInfo(windowInfo)
    {
        this->startX = startX;
        this->startY = startY;

        this->slopeX = slopeX;
        this->slopeY = slopeY;

        this->speed = speed;

        positionX = startX;
        positionY = startY;
    }
    virtual ~Projectile()
    {
    }

    virtual void update()
    {
        float distance = std::sqrt(slopeX * slopeX + slopeY * slopeY);
        float dirX = slopeX / distance;
        float dirY = slopeY / distance;
        float velX = dirX * speed;
        float velY = dirY * speed;

        Point<float> projectileFuturePos;
        projectileFuturePos.x = positionX + velX * windowInfo.DELTA_TIME;
        projectileFuturePos.y = positionY + velY * windowInfo.DELTA_TIME;

        Segment<float> futureVelocity = Segment<float>(Point<float>(positionX, positionY), projectileFuturePos);

        Point<float> intersectPoint;
        bool intersected = false;
        for (EdgeData edge : world->getCollisionMesh())
        {
            SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);
            if (intersectData.intersected)
            {
                intersectPoint.x = intersectData.x;
                intersectPoint.y = intersectData.y;
                intersected = true;
            }
        }

        if (intersected || !rendaer)
        {
            rendaer = false;
            return;
        }

        positionX += velX;
        positionY += velY;

        calculateRotation(startX, startY, startX + slopeX, startY + slopeY);
    }

    void render()
    {
        if (!rendaer)
        {
            return;
        }
        Rect<float> rect(positionX, positionY, 10, 5);
        drawRect(renderWindow, rect, sf::Color(255, 220, 200, 255), DrawMode::Solid, rotation);
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

    bool rendaer = true;

protected:
    sf::RenderWindow &renderWindow;
    World *world;
    WindowInfo &windowInfo;

    float rotation = 0;

    float positionX = 0;
    float positionY = 0;

    float startX = 0;
    float startY = 0;

    float slopeX = 1.0f;
    float slopeY = 1.0f;

    float speed = 0;
};