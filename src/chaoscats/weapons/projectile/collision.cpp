#include "projectile.hpp"

#include <utility>

ProjectileCollideInfo Projectile::checkWorldCollision(float dt)
{
    // get future position
    Vector<float> projectileFuturePos;
    projectileFuturePos.x = position.x + velocity.x * dt;
    projectileFuturePos.y = position.y + velocity.y * dt;

    Segment<float> futureVelocity = Segment<float>(position, projectileFuturePos);

    // check world collision
    for (EdgeData edge : world->getCollisionMesh())
    {
        SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);
        if (intersectData.intersected)
        {
            ProjectileCollideInfo projectileCollideInfo;

            Vector<int> hitPoint = world->getWorldCoords(intersectData.x, intersectData.y);

            projectileCollideInfo.x = hitPoint.x;
            projectileCollideInfo.y = hitPoint.y;
            projectileCollideInfo.projectileSettings = projectileSettings;
            projectileCollideInfo.collideType = CollideType::World;
            projectileCollideInfo.intersected = true;

            hit = true;

            return projectileCollideInfo;
        }
    }

    return ProjectileCollideInfo();
}

ProjectileCollideInfo Projectile::checkPlayerCollision(float dt, PlayerDamageModel &playerHitbox)
{
    // get future position
    Vector<float> projectileFuturePos;
    projectileFuturePos.x = position.x + velocity.x * dt;
    projectileFuturePos.y = position.y + velocity.y * dt;

    Segment<float> futureVelocity = Segment<float>(Vector<float>(position.x, position.y), projectileFuturePos);

    for (DamageModelEdgeData edge : playerHitbox.head->getDamageHitbox())
    {
        SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);
        if (intersectData.intersected)
        {
            ProjectileCollideInfo projectileCollideInfo;

            projectileCollideInfo.x = intersectData.x;
            projectileCollideInfo.y = intersectData.y;
            projectileCollideInfo.projectileSettings = projectileSettings;
            projectileCollideInfo.collideType = CollideType::PlayerHead;
            projectileCollideInfo.intersected = true;

            hit = true;

            return projectileCollideInfo;
        }
    }

    for (DamageModelEdgeData edge : playerHitbox.body->getDamageHitbox())
    {
        SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);
        if (intersectData.intersected)
        {
            ProjectileCollideInfo projectileCollideInfo;

            projectileCollideInfo.x = intersectData.x;
            projectileCollideInfo.y = intersectData.y;
            projectileCollideInfo.projectileSettings = projectileSettings;
            projectileCollideInfo.collideType = CollideType::PlayerBody;
            projectileCollideInfo.intersected = true;

            hit = true;

            return projectileCollideInfo;
        }
    }

    return ProjectileCollideInfo();
}