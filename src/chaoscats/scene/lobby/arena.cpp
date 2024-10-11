#include "arena.hpp"

#include "../../net/data/world.hpp"
#include "../../animated_sprite/animated_sprite.hpp"

int rot = 0;

AnimatedSprite* animatedSprite = nullptr;

CurrentScene Scene_LobbyArena::run()
{
    if (init())
    {
        Logger::log(Logger::LogType::Info, "lobby", "Initializing lobby arena");

        animatedSprite = new AnimatedSprite(gameAssets.textures.ANIMATED_SPRITE_TENNIS, 5, 4, 20);
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
            camera.position_move(-5.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            camera.position_move(5.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            camera.position_move(0, -5.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            camera.position_move(0, 5.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            camera.zoom_move(1.01f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            camera.zoom_move(0.99f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            camera.position_setGoal(0, 0, 0.05f);
            camera.zoom_setGoal(1.0f, 0.05f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            camera.position_setGoal(players[localId]->getPosition().x - gameInfo.window.WIDTH / 2, players[localId]->getPosition().y - gameInfo.window.HEIGHT / 2, 0.05f);
            camera.zoom_setGoal(2.0f, 0.05f);
        }

        world->update(gameInfo.time.DELTA_TIME);
        world->render(renderWindow, &camera);
        camera.update(gameInfo.time.DELTA_TIME);

        animatedSprite->play(24, gameInfo.time.FRAMERATE);
        animatedSprite->render(renderWindow, sf::Vector2f(200, 200), sf::Vector2f(1000, -1), &camera);

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

                player.second->render(renderWindow, &camera);
            }
        }

        // std::string positionInfo = "PX: " +
        //                            std::to_string(camera.getX()) +
        //                            "   PY: " + std::to_string(camera.getY()) +
        //                            "   /   Z: " + std::to_string(camera.getZoom());
        // drawText(renderWindow, positionInfo, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(0, 720 - 30), 15);

        drawRect(renderWindow, Rect<float>(0, 0, 100, 100), &camera, sf::Color(255, 255, 255, 255), DrawMode::Solid, rot);
        // drawTexture(renderWindow, gameAssets.textures.PLAYER_HEAD_TEXTURE, sf::Vector2f(0, 0), sf::Vector2f(100, 100), &camera, rot);

        ++packetSendCounter;
        packetCanSend = false;

        ++rot;
    }

    return CurrentScene::LobbyArena;
}