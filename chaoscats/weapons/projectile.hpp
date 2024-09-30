#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../rendering/draw.hpp"
#include "../window/window_info.hpp"

#include "../math/constants.hpp"

#include "../world/world.hpp"
#include "../world/collision/collision.hpp"
#include "../math/geometry/geometry_common.hpp"

struct ProjectileSettings
{
    float speed = 1.0f;
    int damageRadius = 3;
    int playerDamage = 3;
};

struct ProjectileCollideInfo
{
    int x = 0;
    int y = 0;
    bool isPlayer = false;
    ProjectileSettings projectileSettings;
};

class Projectile
{
public:
    Projectile(sf::RenderWindow &renderWindow,
               WindowInfo &windowInfo,
               const World *world,
               ProjectileSettings projectileSettings,
               Point<float> startPosition,
               Point<float> slope) : renderWindow(renderWindow),
                                     windowInfo(windowInfo),
                                     world(world)
    {
        this->projectileSettings = projectileSettings;

        this->startPosition = startPosition;
        this->position = startPosition;
        this->slope = slope;
    }

    // handle copy
    Projectile &operator=(const Projectile &other)
    {
        if (this != &other)
        {
            this->projectileSettings = other.projectileSettings;

            this->startPosition = other.startPosition;
            this->position = other.position;
            this->slope = other.slope;

            this->hit = other.hit;
        }
        return *this;
    }
    virtual ~Projectile()
    {
    }

    virtual ProjectileCollideInfo update()
    {
        float distance = std::sqrt(slope.x * slope.x + slope.y * slope.y);
        Point<float> direction = Point<float>(slope.x / distance, slope.y / distance);
        Point<float> velocity(direction.x * projectileSettings.speed, direction.y * projectileSettings.speed);

        // get future position
        Point<float> projectileFuturePos;
        projectileFuturePos.x = position.x + velocity.x * windowInfo.DELTA_TIME;
        projectileFuturePos.y = position.y + velocity.y * windowInfo.DELTA_TIME;

        Segment<float> futureVelocity = Segment<float>(Point<float>(position.x, position.y), projectileFuturePos);

        drawSegment(renderWindow, futureVelocity, sf::Color(0, 255, 0, 255));

        // check world intersection
        Point<float> intersectPoint;
        for (EdgeData edge : world->getCollisionMesh())
        {
            SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);
            if (intersectData.intersected)
            {
                intersectPoint.x = intersectData.x;
                intersectPoint.y = intersectData.y;

                ProjectileCollideInfo projectileCollideInfo;

                Point<int> hitPoint = world->getWorldCoords(intersectPoint.x, intersectPoint.y);

                projectileCollideInfo.isPlayer = false;
                projectileCollideInfo.projectileSettings = projectileSettings;
                projectileCollideInfo.x = hitPoint.x;
                projectileCollideInfo.y = hitPoint.y;

                hit = true;

                return projectileCollideInfo;
            }
        }

        position.x += velocity.x * windowInfo.DELTA_TIME;
        position.y += velocity.y * windowInfo.DELTA_TIME;

        return ProjectileCollideInfo();
    }

    virtual void render()
    {
        float rotation = calculateRotation(startPosition.x, startPosition.y, startPosition.x + slope.x, startPosition.y + slope.y);

        Rect<float> rect(position.x, position.y, 10, 5);
        drawRect(renderWindow, rect, sf::Color(255, 220, 200, 255), DrawMode::Solid, rotation);
    }

    virtual bool hasHit() const
    {
        return hit;
    }

    virtual bool outOfBounds() const
    {
        Size<int> worldSize = world->getSize();
        Point<int> worldPos = world->getWorldCoords(position.x, position.y);
        return worldPos.x < 0 || worldPos.y < 0 || worldPos.x > worldSize.w - 1 || worldPos.y > worldSize.h - 1;
    }

protected:
    float calculateRotation(float startX, float startY, float endX, float endY)
    {
        float dx = endX - startX;
        float dy = endY - startY;
        float radians = std::atan2(dy, dx);
        float degrees = (180.0f / PI) * radians;
        float degrees360 = std::fmod(degrees + 360.0f, 360.0f);

        return degrees360;
    }

protected:
    sf::RenderWindow &renderWindow;
    WindowInfo &windowInfo;
    const World *world;

    ProjectileSettings projectileSettings;

    Point<float> position;
    Point<float> startPosition;
    Point<float> slope;

    bool hit = false;
};