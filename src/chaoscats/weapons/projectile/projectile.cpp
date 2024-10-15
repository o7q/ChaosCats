#include "projectile.hpp"

Projectile::Projectile(const World *world,
                       ProjectileSettings projectileSettings,
                       Vector<float> startPosition,
                       Vector<float> slope) : world(world)
{
    this->projectileSettings = projectileSettings;

    this->startPosition = startPosition;
    this->position = startPosition;
    this->slope = slope;

    float distance = std::sqrt(slope.x * slope.x + slope.y * slope.y);
    Vector<float> direction = Vector<float>(slope.x / distance, slope.y / distance);
    this->velocity = Vector<float>(direction.x * projectileSettings.speed, direction.y * projectileSettings.speed);
}

Projectile::~Projectile()
{

}

void Projectile::update(float dt)
{
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void Projectile::render(sf::RenderWindow &renderWindow, GameState &gameState, Camera *camera)
{
    float rotation = degreesFromPoints(startPosition.x, startPosition.y, startPosition.x + slope.x, startPosition.y + slope.y);

    Rect<float> rect(position.x, position.y, 5, 3);
    drawRect(renderWindow, gameState.window, rect, camera, sf::Color(255, 220, 200, 255), DrawMode::Solid, rotation);
}

bool Projectile::hasHit() const
{
    return hit;
}

bool Projectile::outOfBounds() const
{
    Vector<int> worldSize = world->getSize();
    Vector<int> worldPos = world->getWorldCoords(position.x, position.y);
    return worldPos.x < 0 || worldPos.y < 0 || worldPos.x > worldSize.x - 1 || worldPos.y > worldSize.y - 1;
}