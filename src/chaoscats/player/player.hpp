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
        sf::Vector2f sf_head_size = imageAspect(headTextureSize, sf::Vector2f(headTextureSize.x * scale, -1));
        Vector<float> head_size = Vector<float>(sf_head_size.x, sf_head_size.y);
        head_hitbox = Rect<float>(0, 0, head_size.x, head_size.y);

        sf::Vector2u bodyTextureSize = gameAssets.textures.PLAYER_BODY_TEXTURE.getSize();
        sf::Vector2f sf_body_size = imageAspect(bodyTextureSize, sf::Vector2f(bodyTextureSize.x * scale, -1));
        Vector<float> body_size = Vector<float>(sf_body_size.x, sf_body_size.y);
        body_hitbox = Rect<float>(0, 0, body_size.x, body_size.y);

        sf::Vector2u legTextureSize = gameAssets.textures.PLAYER_LEG_TEXTURE.getSize();
        sf::Vector2f sf_leg_size = imageAspect(legTextureSize, sf::Vector2f(legTextureSize.x * scale, -1));
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
        damageModel.head = new DamageModel(headTextureSize.x, headTextureSize.y, headGoreLayers);

        std::vector<sf::Texture *> bodyGoreLayers;
        bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_TEXTURE);
        bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE1_TEXTURE);
        bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE2_TEXTURE);
        bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE3_TEXTURE);
        damageModel.body = new DamageModel(bodyTextureSize.x, bodyTextureSize.y, bodyGoreLayers);

        weapon = new Shotgun(world, gameInfo, gameAssets);
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

        damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
        damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);

        weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    // this function only runs for server replicated clients
    void replicatedClientUpdate()
    {
        damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
        damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);
        weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
    }

    // this function only runs on the server
    void serverUpdate()
    {
        damageModel.head->update(position.x, position.y - head_hitbox.half_size.y - leftLeg_hitbox.half_size.y, scale);
        damageModel.body->update(position.x, position.y + leftLeg_hitbox.half_size.y, scale);
        weapon->update(position.x, position.y, aimPosition.x, aimPosition.y);
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

    void render(sf::RenderWindow &renderWindow, Camera *camera);

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