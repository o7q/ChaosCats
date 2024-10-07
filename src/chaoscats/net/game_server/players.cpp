#include "game_server.hpp"

void GameServer::updatePlayers()
{
    if (!serverPlayers.empty())
    {
        for (const auto &player : serverPlayers)
        {
            updateProjectiles(player.first, player.second);
            player.second->serverUpdate();
        }
    }
}

void GameServer::updateProjectiles(uint32_t id, Player *player)
{
    std::vector<Projectile> &projectiles = player->getActiveWeapon()->getActiveProjectiles();

    for (Projectile &projectile : projectiles)
    {
        for (const auto &otherPlayer : serverPlayers)
        {
            if (id != otherPlayer.first)
            {
                ProjectileCollideInfo playerCollisionInfo = projectile.checkPlayerCollision(serverGameInfo.time.DELTA_TIME, otherPlayer.second->getDamageModel());

                if (playerCollisionInfo.intersected)
                {
                    otherPlayer.second->damage(playerCollisionInfo);
                    net::Message<GameMessages> damagePlayerMessage;
                    damagePlayerMessage.header.id = GameMessages::Server2C_DamagePlayer;
                    damagePlayerMessage << playerCollisionInfo << otherPlayer.first;
                    sendMessageToAllClients(damagePlayerMessage);

                    Particle bloodParticle;
                    bloodParticle.color = sf::Color(120, 6, 6);
                    bloodParticle.physicsType = PhysicsType::Water;
                    bloodParticle.playerCollisionType = PlayerCollisionType::None;
                    bloodParticle.matterType = MatterType::Liquid;

                    worldMutex.lock();
                    Vector<int> worldLoc = world->getWorldCoords(playerCollisionInfo.x, playerCollisionInfo.y);
                    net::Message<GameMessages> bloodMessage = world->drawParticles(worldLoc.x, worldLoc.y, bloodParticle, 3, DrawShape::Circle);
                    worldMutex.unlock();
                    bloodMessage.header.id = GameMessages::Server2C_ModifyWorld;
                    sendMessageToAllClients(bloodMessage);

                    continue;
                }
            }
        }

        // handle world
        Particle particle;
        ProjectileCollideInfo worldCollisionInfo = projectile.checkWorldCollision(serverGameInfo.time.DELTA_TIME);
        worldMutex.lock();
        net::Message<GameMessages> worldModifyMessage = world->drawParticles(worldCollisionInfo.x,
                                                                             worldCollisionInfo.y,
                                                                             particle,
                                                                             worldCollisionInfo.projectileSettings.damageRadius,
                                                                             DrawShape::Circle);
        worldMutex.unlock();
        worldModifyMessage.header.id = GameMessages::Server2C_ModifyWorld;
        sendMessageToAllClients(worldModifyMessage);
    }
}