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
    Player(const World *world, GameInfo &gameInfo, GameAssets &gameAssets) : world(world),
                                                                             gameInfo(gameInfo),
                                                                             gameAssets(gameAssets)
    {
        sf::Vector2u headTextureSize = gameAssets.textures.PLAYER_HEAD_TEXTURE.getSize();
        sf::Vector2f sf_head_size = imageAspect(gameAssets.textures.PLAYER_HEAD_TEXTURE, sf::Vector2f(headTextureSize.x * scale, -1));
        Vector<float> head_size = Vector<float>(sf_head_size.x, sf_head_size.y);
        head_hitbox = Rect<float>(0, 0, head_size.x, head_size.y);

        sf::Vector2u bodyTextureSize = gameAssets.textures.PLAYER_BODY_TEXTURE.getSize();
        sf::Vector2f sf_body_size = imageAspect(gameAssets.textures.PLAYER_BODY_TEXTURE, sf::Vector2f(bodyTextureSize.x * scale, -1));
        Vector<float> body_size = Vector<float>(sf_body_size.x, sf_body_size.y);
        body_hitbox = Rect<float>(0, 0, body_size.x, body_size.y);

        sf::Vector2u legTextureSize = gameAssets.textures.PLAYER_LEG_TEXTURE.getSize();
        sf::Vector2f sf_leg_size = imageAspect(gameAssets.textures.PLAYER_LEG_TEXTURE, sf::Vector2f(legTextureSize.x * scale, -1));
        Vector<float> leftLeg_size = Vector<float>(sf_leg_size.x, sf_leg_size.y);
        leftLeg_hitbox = Rect<float>(0, 0, leftLeg_size.x, leftLeg_size.y);
        Vector<float> rightLeg_size = Vector<float>(sf_leg_size.x, sf_leg_size.y);
        rightLeg_hitbox = Rect<float>(0, 0, rightLeg_size.x, rightLeg_size.y);

        player_collisionBox = Rect<float>(0, 0, body_hitbox.size.x, body_hitbox.size.y + head_hitbox.size.y);

        std::vector<sf::Texture *> headGoreLayers;
        headGoreLayers.push_back(&gameAssets.textures.PLAYER_HEAD_TEXTURE);
        headGoreLayers.push_back(&gameAssets.textures.PLAYER_HEAD_GORE1_TEXTURE);
        headGoreLayers.push_back(&gameAssets.textures.PLAYER_HEAD_GORE2_TEXTURE);
        headGoreLayers.push_back(&gameAssets.textures.PLAYER_HEAD_GORE3_TEXTURE);
        headDamageModel = new DamageModel(headTextureSize.x, headTextureSize.y, headGoreLayers);

        std::vector<sf::Texture *> bodyGoreLayers;
        bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_TEXTURE);
        bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE1_TEXTURE);
        bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE2_TEXTURE);
        bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE3_TEXTURE);
        bodyDamageModel = new DamageModel(bodyTextureSize.x, bodyTextureSize.y, bodyGoreLayers);

        weapon = new Shotgun(world, gameInfo, gameAssets);

        // headDamageModel->damage(5, 6, 8);
        // bodyDamageModel->damage(5, 6, 8);
    }
    virtual ~Player()
    {
        delete weapon;
        weapon = nullptr;
    }

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
    void clientUpdate()
    {
        handleMovement();
        handleCollision();

        setPosition(position.x, position.y);
        if (gameInfo.window.IS_FOCUSED)
        {
            setAim(gameInfo.input.MOUSE_POS.x, gameInfo.input.MOUSE_POS.y);
        }

        headDamageModel->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
        bodyDamageModel->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);

        weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    // this function only runs for server replicated clients
    void visualClientUpdate()
    {
        weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    // this function only runs on the server
    void serverUpdate()
    {
        headDamageModel->update(position.x, position.y, scale);
        bodyDamageModel->update(position.x, position.y, scale);
        projectileCollideInfo.clear();
        projectileCollideInfo = projectileCollideInfo = weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    const std::vector<ProjectileCollideInfo> &getProjectileCollideInfo() const
    {
        return projectileCollideInfo;
    }

    void shoot()
    {
        weapon->shoot(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    void setPosition(float x, float y);

    Vector<float> getPosition() const
    {
        return position;
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

    void render(sf::RenderWindow &renderWindow);

    const Weapon *getActiveWeapon() const
    {
        return weapon;
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
    std::vector<ProjectileCollideInfo> projectileCollideInfo;
    Vector<float> aimPosition = Vector<float>(0, 0);

protected:
    Rect<float> body_hitbox;
    Rect<float> head_hitbox;

    DamageModel *headDamageModel;
    DamageModel *bodyDamageModel;

    Rect<float> leftLeg_hitbox;
    Rect<float> rightLeg_hitbox;

    Rect<float> player_collisionBox;

    Vector<float> bottomCollisionPoint;
    Vector<float> topCollisionPoint;

    bool renderHitboxes = true;

    int scale = 4;
    float playerSpeed = 6.0f;
    bool jumped = false;
};