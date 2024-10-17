#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../world/world.hpp"
#include "../camera/camera.hpp"
#include "../rendering/draw.hpp"

#include "damage_model/damage_model.hpp"

#include "../weapons/weapons/shotgun.hpp"

#include "../math/intersection.hpp"
#include "../math/types/types_common.hpp"

#include "../net/data/player.hpp"

class Player
{
public:
    Player(const World *world, GameState &gameState, GameAssets &gameAssets);
    Player(PlayerData playerData, const World *world, GameState &gameState, GameAssets &gameAssets);
    virtual ~Player();

    void pickupWeapon()
    {
    }

    const Weapon *getWeapon() const
    {
        return weapon;
    }

    // this function only runs for the local player (client-side)
    void clientUpdate(Camera camera);

    // this function only runs for server replicated clients
    void replicatedClientUpdate();

    // this function only runs on the server
    void serverUpdate();

    void shoot(Camera *camera = nullptr)
    {
        Vector<float> kickbackVelocity = weapon->shoot(position.x, position.y, aimPosition.x, aimPosition.y);
        velocity = kickbackVelocity;
        enableXAirVelocity = true;
        if (camera != nullptr)
        {
            camera->shake(20.0f, 10.0f);
        }
    }

    void setPosition(float x, float y);

    Vector<float> getPosition() const
    {
        return position;
    }

    Vector<float> getSize() const
    {
        return player_collisionBox.size;
    }

    void setAim(float x, float y)
    {
        aimPosition.x = x;
        aimPosition.y = y;
    }

    Vector<float> getAim() const
    {
        return aimPosition;
    }

    void render(sf::RenderWindow &renderWindow, Camera *camera, const std::string &name);

    Weapon *getActiveWeapon()
    {
        return weapon;
    }

    PlayerDamageModel &getDamageModel()
    {
        return damageModel;
    }

    void damage(ProjectileCollideInfo collideInfo)
    {
        switch (collideInfo.collideType)
        {
        case CollideType::PlayerHead:
        {
            int x = (collideInfo.x - head_hitbox.getPosition().x + head_hitbox.half_size.x) / scale;
            int y = (collideInfo.y - head_hitbox.getPosition().y + head_hitbox.half_size.y) / scale;
            damageModel.head->damage(x, y, collideInfo.projectileSettings.damageRadius);

            health -= collideInfo.projectileSettings.playerDamage;
        }
        break;
        case CollideType::PlayerBody:
        {
            int x = (collideInfo.x - body_hitbox.getPosition().x + body_hitbox.half_size.x) / scale;
            int y = (collideInfo.y - body_hitbox.getPosition().y + body_hitbox.half_size.y) / scale;
            damageModel.body->damage(x, y, collideInfo.projectileSettings.damageRadius);

            health -= collideInfo.projectileSettings.playerDamage;
        }
        break;
        default:
            break;
        }

        std::cout << "health: " << health << std::endl;
    }

    void setHealth(float health)
    {
        this->health = health;
    }

    float getHealth() const
    {
        return health;
    }

    void kill()
    {
        health -= 10000.0f;
    }

    bool isDead() const
    {
        return health <= 0.0f;
    }

protected:
    void initPlayer();
    void handleMovement();
    void handleCollision();

    const World *world = nullptr;
    GameState &gameState;
    GameAssets &gameAssets;

protected:
    Vector<float> position = Vector<float>(0, 0);
    Vector<float> velocity = Vector<float>(0, 0);

protected:
    Weapon *weapon = nullptr;
    Vector<float> aimPosition = Vector<float>(0, 0);

protected:
    Rect<float> body_hitbox;
    Rect<float> head_hitbox;

    PlayerDamageModel damageModel;

    Rect<float> leftLeg_hitbox;
    Rect<float> rightLeg_hitbox;

    Rect<float> player_collisionBox;

    Segment<float> collisionSegment;

    bool renderHitboxes = true;

    int scale = 3;
    bool jumped = false;

    bool enableXAirVelocity = false;

    float health = 100.0f;
};