#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../weapons/weapons/shotgun.hpp"

#include "../world/world.hpp"
#include "../world/collision/collision.hpp"

#include "../math/geometry/geometry_common.hpp"
#include "../rendering/draw.hpp"

#include "damage_model/damage_model.hpp"

class Player
{
public:
    Player(const World *world, GameInfo &gameInfo, GameAssets &gameAssets);
    virtual ~Player();

    void pickupWeapon()
    {
    }

    const Weapon *getWeapon() const
    {
        return weapon;
    }

    void handleMovement();
    void handleCollision();

    // this function only runs for the local player (client-side)
    void clientUpdate();

    // this function only runs for server replicated clients
    void replicatedClientUpdate();

    // this function only runs on the server
    void serverUpdate();

    void shoot()
    {
        weapon->shoot(position.x, position.y, aimPosition.x, aimPosition.y);
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

    void render(sf::RenderWindow &renderWindow, Camera *camera, const std::string& name);

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
        }
        break;
        case CollideType::PlayerBody:
        {
            int x = (collideInfo.x - body_hitbox.getPosition().x + body_hitbox.half_size.x) / scale;
            int y = (collideInfo.y - body_hitbox.getPosition().y + body_hitbox.half_size.y) / scale;
            damageModel.body->damage(x, y, collideInfo.projectileSettings.damageRadius);
        }
        break;
        default:
            break;
        }
    }

protected:
    const World *world = nullptr;
    GameInfo &gameInfo;
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

    Vector<float> bottomCollisionPoint;
    Vector<float> topCollisionPoint;

    bool renderHitboxes = true;

    int scale = 3;
    float playerSpeed = 6.0f;
    bool jumped = false;
};