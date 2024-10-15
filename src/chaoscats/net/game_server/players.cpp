#include "game_server.hpp"

void GameServer::updatePlayers()
{
    std::lock_guard playersLock(playersMutex);
    if (!serverPlayers.empty())
    {
        for (const auto &player : serverPlayers)
        {
            player.second->serverUpdate();
        }
    }
}

void GameServer::updateProjectiles()
{
    std::lock_guard<std::shared_mutex> playersLock(playersMutex);
    std::lock_guard<std::shared_mutex> worldLock(worldMutex);
    if (!serverPlayers.empty())
    {
        for (const auto &player : serverPlayers)
        {
            std::vector<Projectile> &projectiles = player.second->getActiveWeapon()->getActiveProjectiles();

            for (const auto &otherPlayer : serverPlayers)
            {
                if (player.first != otherPlayer.first)
                {
                    for (Projectile &projectile : projectiles)
                    {
                        ProjectileCollideInfo pci = projectile.checkPlayerCollision(serverGameState.time.DELTA_TIME, otherPlayer.second->getDamageModel());

                        if (pci.intersected)
                        {
                            otherPlayer.second->damage(pci);
                            net::Message<GameMessages> damagePlayerMessage;
                            damagePlayerMessage.header.id = GameMessages::Server2C_DamagePlayer;
                            damagePlayerMessage << pci << otherPlayer.first;
                            sendMessageToAllClients(damagePlayerMessage);

                            Particle bloodParticle;
                            bloodParticle.color = sf::Color(120, 6, 6);
                            bloodParticle.physicsType = PhysicsType::Water;
                            bloodParticle.playerCollisionType = PlayerCollisionType::None;
                            bloodParticle.matterType = MatterType::Liquid;

                            Vector<int> worldLoc = world->getWorldCoords(pci.x, pci.y);
                            net::Message<GameMessages> bloodMessage = world->drawParticles(worldLoc.x, worldLoc.y, bloodParticle, 3, DrawShape::Circle);
                            bloodMessage.header.id = GameMessages::Server2C_ModifyWorld;
                            sendMessageToAllClients(bloodMessage);
                        }
                    }
                }
            }

            for (Projectile &projectile : projectiles)
            {
                ProjectileCollideInfo worldCollisionInfo = projectile.checkWorldCollision(serverGameState.time.DELTA_TIME);
                if (worldCollisionInfo.intersected)
                {
                    Particle particle;
                    net::Message<GameMessages> worldModifyMessage = world->drawParticles(worldCollisionInfo.x,
                                                                                         worldCollisionInfo.y,
                                                                                         particle,
                                                                                         worldCollisionInfo.projectileSettings.damageRadius,
                                                                                         DrawShape::Circle);

                    worldModifyMessage.header.id = GameMessages::Server2C_ModifyWorld;
                    sendMessageToAllClients(worldModifyMessage);
                }
            }
        }
    }
}