#include "arena.hpp"

#include "../../net/data/world.hpp"
#include "../../animated_sprite/animated_sprite.hpp"

int rot = 0;

AnimatedSprite *animatedSprite = nullptr;

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
        if (!world->is_s2c_initialized())
        {
            world->render(renderWindow, gameState.window, &camera);
            return CurrentScene::LobbyArena;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && gameState.window.IS_FOCUSED)
        {
            camera.position_move(-5.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && gameState.window.IS_FOCUSED)
        {
            camera.position_move(5.0f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && gameState.window.IS_FOCUSED)
        {
            camera.position_move(0, -5.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && gameState.window.IS_FOCUSED)
        {
            camera.position_move(0, 5.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && gameState.window.IS_FOCUSED)
        {
            camera.zoom_move(1.01f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && gameState.window.IS_FOCUSED)
        {
            camera.zoom_move(0.99f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && gameState.window.IS_FOCUSED)
        {
            gameState.time.setGameSpeed(0.1f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V) && gameState.window.IS_FOCUSED)
        {
            gameState.time.setGameSpeed(0.5f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && gameState.window.IS_FOCUSED)
        {
            gameState.time.setGameSpeed(2.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && gameState.window.IS_FOCUSED)
        {
            gameState.time.setGameSpeed(1.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && gameState.window.IS_FOCUSED)
        {
            camera.position_setGoal(0, 0, 0.05f);
            camera.zoom_setGoal(0.5f, 0.05f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && gameState.window.IS_FOCUSED)
        {
            camera.position_setGoal(
                players[localId]->getPosition().x - gameState.window.WIDTH / 2 - players[localId]->getSize().x / 2,
                players[localId]->getPosition().y - gameState.window.HEIGHT / 2 - players[localId]->getSize().y / 2,
                0.05f);
            camera.zoom_setGoal(1.5f, 0.05f);
        }

        world->update(gameState.time.DELTA_TIME, gameState.time.BASE_DELTA_TIME);
        world->render(renderWindow, gameState.window, &camera);

        animatedSprite->play(24, gameState.time.DELTA_TIME);
        animatedSprite->render(renderWindow, gameState.window, sf::Vector2f(200, 200), sf::Vector2f(1000, -1), &camera);

        drawText(renderWindow, gameState.window, "hello", gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(200, 200), 20, &camera, sf::Color(255, 255, 255, 255));

        // packet limiter, only allows packets to send at 'packetSendRate'
        if (packetSendCounter > gameState.time.FRAMERATE / packetSendRate)
        {
            packetCanSend = true;
            packetSendCounter = 0;
        }

        // update local player
        if (mapContains(players, localId))
        {
            players[localId]->clientUpdate(camera);

            std::vector<Projectile> &activeProjectiles = players[localId]->getActiveWeapon()->getActiveProjectiles();

            for (Projectile &projectile : activeProjectiles)
            {
                projectile.checkWorldCollision(gameState.time.DELTA_TIME);

                for (auto &player : players)
                {
                    if (player.first != localId)
                    {
                        projectile.checkPlayerCollision(gameState.time.DELTA_TIME, player.second->getDamageModel());
                    }
                }
            }

            // packet senders in this conditional will be sent at a constant rate: 'packetSendRate'
            if (packetCanSend)
            {
                client_player_action_handleMove();
            }

            if (gameState.input.MOUSE_LMB_CLICKED && gameState.window.IS_FOCUSED)
            {
                players[localId]->shoot(&camera);
                client_player_action_handleShoot();
            }
        }

        std::vector<Vector<float>> positions;

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
                        projectile.checkWorldCollision(gameState.time.DELTA_TIME);

                        for (auto &otherPlayer : players)
                        {
                            if (player.first != otherPlayer.first)
                            {
                                projectile.checkPlayerCollision(gameState.time.DELTA_TIME, otherPlayer.second->getDamageModel());
                            }
                        }
                    }
                }

                if (!player.second->isDead())
                {
                    positions.push_back(player.second->getPosition());
                }

                player.second->render(renderWindow, &camera, "Player" + std::to_string(player.first));
            }
        }

        camera.update(gameState.time.DELTA_TIME);
        camera.focus_positions(positions, gameState.window);

        std::string positionInfo = "CX: " +
                                   std::to_string(camera.getX()) +
                                   "   CY: " + std::to_string(camera.getY()) +
                                   "   /   Z: " + std::to_string(camera.getZoom());
        drawText(renderWindow, gameState.window, positionInfo, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(0, 720 - 30), 15, nullptr, sf::Color(255, 255, 0, 255));

        ++packetSendCounter;
        packetCanSend = false;

        ++rot;
    }

    return CurrentScene::LobbyArena;
}