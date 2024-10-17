#include "shotgun.hpp"

#include "../../math/random.hpp"

Vector<float> Shotgun::shoot(float playerX, float playerY, float aimX, float aimY)
{
    int shellCount = 10;
    float spreadAngle = 15.0f;
    float halfSpreadAngle = spreadAngle / 2.0f;

    Vector<float> mp = rotatePoint(Vector<float>(
                                       muzzlePosition.x * scale - weaponSize.x / 2,
                                       muzzlePosition.y * scale - weaponSize.y / 2),
                                   rotation_radians);

    float x_new = mp.x + playerX;
    float y_new = mp.y + playerY;

    float dx = aimX - x_new;
    float dy = aimY - y_new;

    float baseAngle = radiansFromPoints(x_new, y_new, aimX, aimY);

    ProjectileSettings projectileSettings;
    projectileSettings.damageRadius = 3.0f;
    projectileSettings.playerDamage = 5.0f;
    projectileSettings.speed = 110.0f;

    for (int i = 0; i < shellCount; ++i)
    {
        float angleOffset = i * (spreadAngle / (shellCount - 1)) - halfSpreadAngle;

        Random::LCG lcg(std::pow(bulletSprayIndex, i));
        int rand = lcg.next_in_range(-5, 5);

        float newAngle = baseAngle + degreesToRadians(angleOffset + rand);

        float new_dx = std::cos(newAngle);
        float new_dy = std::sin(newAngle);

        Projectile projectile(world, projectileSettings, Vector<float>(x_new, y_new), Vector<float>(new_dx, new_dy));
        projectiles.push_back(projectile);
    }

    float distance = std::sqrt(dx * dx + dy * dy);
    float dirX = dx / distance;
    float dirY = dy / distance;

    ++bulletSprayIndex;

    if (bulletSprayIndex > 10)
    {
        bulletSprayIndex = 0;
    }

    return Vector<float>(-dirX * 30, -dirY * 10);
}

void Shotgun::update(float playerX, float playerY, float aimX, float aimY)
{
    for (Projectile &projectile : projectiles)
    {
        projectile.update(gameState.time.DELTA_TIME);
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const Projectile &projectile)
                       {
                           return projectile.hasHit() || projectile.outOfBounds();
                       }),
        projectiles.end());

    rotation_radians = radiansFromPoints(playerX, playerY, aimX, aimY);
    rotation_degrees = degreesFromPoints(playerX, playerY, aimX, aimY);
}

void Shotgun::render(sf::RenderWindow &renderWindow, Camera *camera, float x, float y, float aimX, float aimY)
{
    weaponPositionOffset.x = std::fabs(weaponPositionOffset.x);
    weaponPositionOffset.y = std::fabs(weaponPositionOffset.y);

    sf::Vector2i rotateVector(1, 1);
    if (rotation_degrees > 90 && rotation_degrees < 270)
    {
        rotateVector = sf::Vector2i(1, -1);
        weaponPositionOffset.x *= -1;
    }

    for (Projectile &projectile : projectiles)
    {
        projectile.render(renderWindow, gameState, camera);
    }

    drawTexture(renderWindow,
                gameState.window,
                gameAssets.textures.WEAPON_SHOTGUN_TEXTURE,
                sf::Vector2f(x + weaponPositionOffset.x,
                             y + weaponPositionOffset.y),
                sf::Vector2f(weaponSize.x, weaponSize.y),
                camera,
                rotation_degrees,
                rotateVector);
}