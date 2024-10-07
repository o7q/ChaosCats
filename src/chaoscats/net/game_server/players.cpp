#include "game_server.hpp"

void GameServer::updatePlayers()
{
    if (!serverPlayers.empty())
    {
        for (const auto &player : serverPlayers)
        {
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

                    {
                        std::scoped_lock<std::mutex> lock(worldMutex);
                        Vector<int> worldLoc = world->getWorldCoords(playerCollisionInfo.x, playerCollisionInfo.y);
                        net::Message<GameMessages> bloodMessage = world->drawParticles(worldLoc.x, worldLoc.y, bloodParticle, 3, DrawShape::Circle);
                        bloodMessage.header.id = GameMessages::Server2C_ModifyWorld;
                        sendMessageToAllClients(bloodMessage);
                    }

                    continue;
                }
            }
        }

        // handle world
        {
            std::scoped_lock lock(worldMutex);
            Particle particle;
            ProjectileCollideInfo worldCollisionInfo = projectile.checkWorldCollision(serverGameInfo.time.DELTA_TIME);
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