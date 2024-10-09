#include "arena.hpp"

#include "../../net/data/world.hpp"
#include "../../animated_sprite/animated_sprite.hpp"

CurrentScene Scene_LobbyArena::run()
{
    if (init())
    {
        Logger::log(Logger::LogType::Info, "lobby", "Initializing lobby arena");

        worldCamera = new WorldCamera();
    }

    if (update())
    {
        if (!client.isConnectedToServer())
        {
            return CurrentScene::MainMenu;
        }
        handleIncomingPackets();

        if (world == nullptr)
        {
            return CurrentScene::LobbyArena;
        }
        if (!world->isInitialized())
        {
            return CurrentScene::LobbyArena;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            worldCamera->translate(-5.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            worldCamera->translate(5.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            worldCamera->translate(0, -5.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            worldCamera->translate(0, 5.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            worldCamera->zoom(1.01f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            worldCamera->zoom(0.99f);
        }

        world->update(gameInfo.time.DELTA_TIME);
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

                player.second->render(renderWindow, worldCamera);
            }
        }

        std::string positionInfo = "PX: " +
                                   std::to_string(worldCamera->getTranslateX()) +
                                   "   PY: " + std::to_string(worldCamera->getTranslateY()) +
                                   "   /   Z: " + std::to_string(worldCamera->getZoom());
        drawText(renderWindow, positionInfo, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(0, 720 - 30), 15);

        ++packetSendCounter;
        packetCanSend = false;
    }

    return CurrentScene::LobbyArena;
}