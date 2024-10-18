#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../../rendering/draw.hpp"
#include "../../game_state/game_state.hpp"

#include "../../math/constants.hpp"
#include "../../math/angle.hpp"

#include "../../world/world.hpp"
#include "../../math/intersection.hpp"
#include "../../math/types/types_common.hpp"

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
               Vector<float> slope);

    virtual ~Projectile();

    // handle copy
    Projectile &operator=(const Projectile &other)
    {
        if (this != &other)
        {
            this->world = other.world;
            this->projectileSettings = other.projectileSettings;

            this->position = other.position;
            this->startPosition = other.startPosition;
            this->slope = other.slope;

            this->velocity = other.velocity;

            this->hit = other.hit;
        }
        return *this;
    }

    virtual void update(float dt);
    virtual void render(sf::RenderWindow &renderWindow, const GameState &gameState, const Camera *camera);

    virtual bool hasHit() const;
    virtual bool outOfBounds() const;

    virtual ProjectileCollideInfo checkWorldCollision(float dt);
    virtual ProjectileCollideInfo checkPlayerCollision(float dt, PlayerDamageModel &playerHitbox);

protected:
    const World *world;

    ProjectileSettings projectileSettings;

    Vector<float> position;
    Vector<float> startPosition;
    Vector<float> slope;

    Vector<float> velocity;

    bool hit = false;
};