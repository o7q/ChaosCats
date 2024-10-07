#include "arena.hpp"

#include "../../net/data/world.hpp"

CurrentScene Scene_LobbyArena::run()
{
    if (init())
    {
        Logger::log(Logger::LogType::Info, "lobby", "Initializing lobby arena");

        worldCamera = new WorldCamera();
        world = new World(gameAssets.maps.TEST_MAP, 5);
    }

    if (update())
    {
        if (!client.isConnectedToServer())
        {
            return CurrentScene::MainMenu;
        }
        handleIncomingPackets();

        world->update();
        world->render(worldCamera, renderWindow, gameInfo.window.WIDTH, gameInfo.window.HEIGHT);

        // packet limiter, only allows packets to send at 'packetSendRate'
        if (packetSendCounter > gameInfo.time.FRAMERATE / packetSendRate)
        {
            packetCanSend = true;
            packetSendCounter = 0;
        }

        // update local player
        if (mapContains(players, localId))
        {
            players[localId]->clientUpdate();

            std::vector<Projectile> &activeProjectiles = players[localId]->getActiveWeapon()->getActiveProjectiles();

            for (Projectile &projectile : activeProjectiles)
            {
                projectile.checkWorldCollision(gameInfo.time.DELTA_TIME);

                for (auto &player : players)
                {
                    projectile.checkPlayerCollision(gameInfo.time.DELTA_TIME, player.second->getDamageModel());
                }
            }

            // packet senders in this conditional will be sent at a constant rate: 'packetSendRate'
            if (packetCanSend)
            {
                client_player_action_handleMove();
            }

            if (gameInfo.input.MOUSE_LMB_CLICKED && gameInfo.window.IS_FOCUSED)
            {
                players[localId]->shoot();
                client_player_action_handleShoot();
            }
        }

        if (!players.empty())
        {
            for (const auto &player : players)
            {
                // only update replicated clients
                if (player.first != localId)
                {
                    player.second->replicatedClientUpdate();

                    std::vector<Projectile> &activeProjectiles = player.second->getActiveWeapon()->getActiveProjectiles();

                    for (Projectile &projectile : activeProjectiles)
                    {
                        projectile.checkWorldCollision(gameInfo.time.DELTA_TIME);

                        for (auto &otherPlayer : players)
                        {
                            if (player.first != otherPlayer.first)
                            {
                                projectile.checkPlayerCollision(gameInfo.time.DELTA_TIME, otherPlayer.second->getDamageModel());
                            }
                        }
                    }
                }

                player.second->render(renderWindow);
            }
        }

        std::string positionInfo = "PX: " +
                                   std::to_string(worldCamera->getTranslateX()) +
                                   "   PY: " + std::to_string(worldCamera->getTranslateY()) +
                                   "   /   ZX: " + std::to_string(worldCamera->getZoomX()) +
                                   "   ZY: " + std::to_string(worldCamera->getZoomY());
        drawText(renderWindow, positionInfo, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(0, 720 - 30), 15);

        ++packetSendCounter;
        packetCanSend = false;
    }

    return CurrentScene::LobbyArena;
}