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

#include "../entity/entity.hpp"

class Player : public Entity
{
public:
    Player(const GameState &gameState, const World *world, const GameAssets &gameAssets);
    Player(const PlayerData &playerData, const GameState &gameState, const World *world, const GameAssets &gameAssets);
    virtual ~Player();

    // these function only runs for the local player (client-side)
    void clientUpdate(Camera camera);
    void render(sf::RenderWindow &renderWindow, Camera *camera, const std::string &name);

    // this function only runs for server replicated clients
    void replicatedClientUpdate();

    // this function only runs on the server
    void serverUpdate();

    void updateChildPositions() override;

    void setAim(float x, float y)
    {
        aimPosition.x = x;
        aimPosition.y = y;

        // determine if the player is facing right based off of their aim position
        facingRight = aimPosition.x > position.x;
    }

    Vector<float> getAim() const
    {
        return aimPosition;
    }

    void shoot(Camera *camera = nullptr);
    Weapon *getActiveWeapon();

    PlayerDamageModel &getDamageModel();
    void damage(ProjectileCollideInfo collideInfo);

    void setHealth(float health);
    float getHealth() const;
    void kill();
    bool isDead() const;

private:
    void init();

    void handleMovement();
    void handleCollision();

private:
    const GameAssets &gameAssets;

private:
    Vector<float> aimPosition = Vector<float>(0, 0);

    // if true, a smooth x-velocity falloff will be applied to the player
    // this is used for weapon kickback velocity
    // we don't always want the x-velocity to smoothly falloff, thus this boolean
    bool enableXAirVelocity = false;
    bool jumped = false;
    // used to change direction of player texture
    bool facingRight = true;

private:
    Weapon *weapon = nullptr;

private:
    Rect<float> body_boundingBox;
    Rect<float> head_boundingBox;

    Rect<float> leftLeg_boundingBox;
    Rect<float> rightLeg_boundingBox;

    // dynamic player hitbox
    PlayerDamageModel damageModel;

    // player scale
    int scale = 3;

private:
    float health = 100.0f;

private:
    bool renderDebug = true;
};