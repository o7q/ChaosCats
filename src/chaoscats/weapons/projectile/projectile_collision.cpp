#include "projectile.hpp"

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

            float x_diff = intersectData.x - startPosition.x;
            float y_diff = intersectData.y - startPosition.y;

            float distance = std::sqrt(x_diff * x_diff + y_diff * y_diff);

            float new_damage = projectileSettings.playerDamage * (1000.0f / (distance + 1));

            projectileCollideInfo.projectileSettings.playerDamage = new_damage;

            return projectileCollideInfo;
        }
    }

    // for (DamageModelEdgeData edge : playerHitbox.body->getDamageHitbox())
    // {
    //     SS_IntersectData intersectData = segmentSegment(futureVelocity, edge.segment);
    //     if (intersectData.intersected)
    //     {
    //         ProjectileCollideInfo projectileCollideInfo;

    //         projectileCollideInfo.x = intersectData.x;
    //         projectileCollideInfo.y = intersectData.y;
    //         projectileCollideInfo.projectileSettings = projectileSettings;
    //         projectileCollideInfo.collideType = CollideType::PlayerBody;
    //         projectileCollideInfo.intersected = true;

    //         hit = true;

    //         float x_diff = position.x - startPosition.x;
    //         float y_diff = position.y - startPosition.y;

    //         float distance = std::sqrt(x_diff * x_diff + y_diff * y_diff);

    //         float new_damage = projectileSettings.playerDamage * (10.0f / (distance + 1));

    //         projectileCollideInfo.projectileSettings.playerDamage = new_damage;

    //         return projectileCollideInfo;
    //     }
    // }

    return ProjectileCollideInfo();
}