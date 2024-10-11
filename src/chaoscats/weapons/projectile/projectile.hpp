#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../../rendering/draw.hpp"
#include "../../game_info/game_info.hpp"

#include "../../math/constants.hpp"
#include "../../math/trig.hpp"

#include "../../world/world.hpp"
#include "../../world/collision/collision.hpp"
#include "../../math/geometry/geometry_common.hpp"

#include "../../player/damage_model/damage_model.hpp"

struct ProjectileSettings
{
    float speed = 1.0f;
    int damageRadius = 3;
    int playerDamage = 3;
};

enum class CollideType
{
    World,
    PlayerHead,
    PlayerBody
};

struct ProjectileCollideInfo
{
    int x = 0;
    int y = 0;
    bool intersected = false;
    ProjectileSettings projectileSettings;
    CollideType collideType;
};

class Projectile
{
public:
    Projectile(const World *world,
               ProjectileSettings projectileSettings,
               Vector<float> startPosition,
               Vector<float> slope) : world(world)
    {
        this->projectileSettings = projectileSettings;

        this->startPosition = startPosition;
        this->position = startPosition;
        this->slope = slope;

        float distance = std::sqrt(slope.x * slope.x + slope.y * slope.y);
        Vector<float> direction = Vector<float>(slope.x / distance, slope.y / distance);
        this->velocity = Vector<float>(direction.x * projectileSettings.speed, direction.y * projectileSettings.speed);
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

            this->velocity = other.velocity;

            this->hit = other.hit;
        }
        return *this;
    }
    virtual ~Projectile()
    {
    }

    virtual void update(float dt)
    {
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
    }

    virtual ProjectileCollideInfo checkWorldCollision(float dt);
    virtual ProjectileCollideInfo checkPlayerCollision(float dt, PlayerDamageModel &playerHitbox);

    virtual void render(sf::RenderWindow &renderWindow)
    {
        float rotation = degreesFromPoints(startPosition.x, startPosition.y, startPosition.x + slope.x, startPosition.y + slope.y);

        Rect<float> rect(position.x, position.y, 10, 5);
        // drawRect(renderWindow, rect, sf::Color(255, 220, 200, 255), DrawMode::Solid, rotation);
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

    ProjectileSettings projectileSettings;

    Vector<float> position;
    Vector<float> startPosition;
    Vector<float> slope;

    Vector<float> velocity;

    bool hit = false;
};