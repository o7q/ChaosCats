#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../rendering/draw.hpp"
#include "../game_info/game_info.hpp"

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
    Projectile(const World *world,
            // std::vector<Player *> &players,
               ProjectileSettings projectileSettings,
               Vector<float> startPosition,
               Vector<float> slope) : /*players(players),*/
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

    virtual ProjectileCollideInfo update(float dt)
    {
        float distance = std::sqrt(slope.x * slope.x + slope.y * slope.y);
        Vector<float> direction = Vector<float>(slope.x / distance, slope.y / distance);
        Vector<float> velocity(direction.x * projectileSettings.speed, direction.y * projectileSettings.speed);

        // get future position
        Vector<float> projectileFuturePos;
        projectileFuturePos.x = position.x + velocity.x * dt;
        projectileFuturePos.y = position.y + velocity.y * dt;

        Segment<float> futureVelocity = Segment<float>(Vector<float>(position.x, position.y), projectileFuturePos);

        // check world intersection
        Vector<float> intersectPoint;
        for (EdgeData edge : world->getCollisionMesh())
        {
            SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);
            if (intersectData.intersected)
            {
                intersectPoint.x = intersectData.x;
                intersectPoint.y = intersectData.y;

                ProjectileCollideInfo projectileCollideInfo;

                Vector<int> hitPoint = world->getWorldCoords(intersectPoint.x, intersectPoint.y);

                projectileCollideInfo.isPlayer = false;
                projectileCollideInfo.projectileSettings = projectileSettings;
                projectileCollideInfo.x = hitPoint.x;
                projectileCollideInfo.y = hitPoint.y;

                hit = true;

                return projectileCollideInfo;
            }
        }

        // for (Player *player : players)
        // {
        //     for 
        // }

        position.x += velocity.x * dt;
        position.y += velocity.y * dt;

        return ProjectileCollideInfo();
    }

    virtual void render(sf::RenderWindow &renderWindow)
    {
        float rotation = degreesFromPoints(startPosition.x, startPosition.y, startPosition.x + slope.x, startPosition.y + slope.y);

        Rect<float> rect(position.x, position.y, 10, 5);
        drawRect(renderWindow, rect, sf::Color(255, 220, 200, 255), DrawMode::Solid, rotation);
    }

    virtual bool hasHit() const
    {
        return hit;
    }

    virtual bool outOfBounds() const
    {
        Vector<int> worldSize = world->getSize();
        Vector<int> worldPos = world->getWorldCoords(position.x, position.y);
        return worldPos.x < 0 || worldPos.y < 0 || worldPos.x > worldSize.x - 1 || worldPos.y > worldSize.y - 1;
    }

protected:
    const World *world;
    // const std::vector<Player *> &players;

    ProjectileSettings projectileSettings;

    Vector<float> position;
    Vector<float> startPosition;
    Vector<float> slope;

    bool hit = false;
};