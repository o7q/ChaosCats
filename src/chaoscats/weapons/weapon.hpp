#pragma once

#include <iostream>
#include <cmath>
#include <utility>

#include <SFML/Graphics.hpp>

#include "projectile/projectile.hpp"

#include "../rendering/draw.hpp"
#include "../game_state/game_state.hpp"

#include "../math/constants.hpp"

#include "../world/world.hpp"

#include "../game_assets/game_assets.hpp"

class Weapon
{
public:
    Weapon(const World *world,
           const GameState &gameState,
           const GameAssets &gameAssets) : world(world),
                                     gameState(gameState),
                                     gameAssets(gameAssets)
    {
    }
    virtual ~Weapon()
    {
    }

    virtual Vector<float> shoot(float playerX, float playerY, float aimX, float aimY)
    {
        return Vector<float>(0, 0);
    }

    virtual void update(float playerX, float playerY, float aimX, float aimY)
    {
    }

    virtual void render(sf::RenderWindow &renderWindow, Camera *camera, float playerX, float playerY, float aimX, float aimY)
    {
    }

    virtual void initHandPlacementPoint(const sf::Texture &weaponTexture, const sf::Texture &weaponPointsTexture);
    virtual std::pair<Vector<float>, Vector<float>> getHandPlacementPoints(float x, float y);

    virtual std::vector<Projectile> &getActiveProjectiles()
    {
        return projectiles;
    }

protected:
    const World *world;
    const GameState &gameState;
    const GameAssets &gameAssets;

    std::vector<Projectile> projectiles;

    Vector<int> handMountPosition1;
    Vector<int> handMountPosition2;
    Vector<int> muzzlePosition;

    float scale = 3.0f;
    Vector<float> weaponSize;
    Vector<float> weaponPositionOffset;

    float rotation_radians = 0.0f;
    float rotation_degrees = 0.0f;

    float playerX = 0;
    float playerY = 0;
    float aimX = 0;
    float aimY = 0;

    float aimAngle;

    int bulletSprayIndex = 0;
};