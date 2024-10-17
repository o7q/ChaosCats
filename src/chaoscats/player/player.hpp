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

    // this function only runs for the local player (client-side)
    void clientUpdate(Camera camera);

    // this function only runs for server replicated clients
    void replicatedClientUpdate();

    // this function only runs on the server
    void serverUpdate();

    void setPosition(float x, float y);

    Vector<float> getPosition() const
    {
        return position;
    }

    Vector<float> getSize() const
    {
        return player_collisionBox.size;
    }

    void shoot(Camera *camera = nullptr);

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

    Weapon *getActiveWeapon();

    PlayerDamageModel &getDamageModel();
    void damage(ProjectileCollideInfo collideInfo);

    void setHealth(float health);
    float getHealth() const;
    void kill();
    bool isDead() const;

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

    bool facingRight = true;

    float health = 100.0f;
};