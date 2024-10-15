#pragma once

#include <iostream>
#include <utility>

#include <SFML/Graphics.hpp>

#include "../../rendering/draw.hpp"
#include "../../math/angle.hpp"
#include "../weapon.hpp"

#include "../../camera/camera.hpp"

class Shotgun : public Weapon
{
public:
    Shotgun(const World *world,
            GameState &gameState,
            GameAssets &gameAssets) : Weapon(world,
                                             gameState,
                                             gameAssets)
    {
        scale = 3.0f;
        initHandPlacementPoint(gameAssets.textures.WEAPON_SHOTGUN_TEXTURE, gameAssets.textures.WEAPON_SHOTGUN_POINTS_TEXTURE);

        weaponPositionOffset.x = 10;
        weaponPositionOffset.y = 5;
    }
    virtual ~Shotgun() override
    {
    }

    Vector<float> shoot(float playerX, float playerY, float aimX, float aimY) override;
    virtual void update(float playerX, float playerY, float aimX, float aimY) override;
    void render(sf::RenderWindow &renderWindow, Camera *camera, float x, float y, float aimX, float aimY) override;
};