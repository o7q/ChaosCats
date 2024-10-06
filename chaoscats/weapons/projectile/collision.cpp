#include "projectile.hpp"

#include <utility>

ProjectileCollideInfo Projectile::checkWorldCollision(float dt, Segment<float> futureVelocity)
{
    // get future position
    Vector<float> projectileFuturePos;
    projectileFuturePos.x = position.x + velocity.x * dt;
    projectileFuturePos.y = position.y + velocity.y * dt;

    Segment<float> futureVelocity = Segment<float>(Vector<float>(position.x, position.y), projectileFuturePos);

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

            hit = true;

            return projectileCollideInfo;
        }
    }
}

ProjectileCollideInfo Projectile::checkPlayerCollision(Segment<float> futureVelocity, std::vector<Player *> &players);
{
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

            hit = true;

            return projectileCollideInfo;
        }
    }
}
