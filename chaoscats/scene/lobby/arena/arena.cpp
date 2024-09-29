#include "arena.hpp"

#include "../../../net/data/world.hpp"

CurrentScene Scene_LobbyArena::run()
{
    if (init())
    {
        Logger::log(Logger::LogType::Info, "lobby", "Initializing lobby arena");

        worldCamera = new WorldCamera();
        world = new World(renderWindow, windowInfo, ASSETS.MAPS.TEST_MAP, 5);
    }

    if (update())
    {
        handleIncomingPackets();

        // Particle drawParticle;
        // drawParticle.color = sf::Color(255, 255, 0, 255);
        // drawParticle.matterType = MatterType::Solid;
        // drawParticle.physicsType = PhysicsType::Sand;
        // drawParticle.playerCollisionType = PlayerCollisionType::Solid;
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        // {
        //     drawParticle.color = sf::Color(0, 0, 0, 0);
        //     drawParticle.matterType = MatterType::None;
        //     drawParticle.physicsType = PhysicsType::None;
        //     drawParticle.playerCollisionType = PlayerCollisionType::None;
        // }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        // {
        //     drawParticle.color = sf::Color(220, 0, 0, 255);
        //     drawParticle.matterType = MatterType::Liquid;
        //     drawParticle.physicsType = PhysicsType::Water;
        //     drawParticle.playerCollisionType = PlayerCollisionType::None;
        // }

        // if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && windowInfo.IS_FOCUSED)
        // {
        //     sf::Vector2i localMouse = sf::Mouse::getPosition(renderWindow);
        //     sf::Vector2f mouseLoc;
        //     worldCamera->screenToWorld(localMouse.x, localMouse.y, mouseLoc.x, mouseLoc.y);

        //     net::Message<GameMessages> updateWorldMessage = world->drawParticles(mouseLoc.x / 5, mouseLoc.y / 5, drawParticle, 2);
        //     updateWorldMessage.header.id = GameMessages::Client2S_ModifyWorld;
        //     client.sendMessageToServer(updateWorldMessage);
        // }

        world->update();
        world->render(worldCamera);

        // packet limiter, only allows packets to send at 'packetSendRate'
        if (packetSendCounter > windowInfo.FRAMERATE / packetSendRate)
        {
            packetCanSend = true;
            packetSendCounter = 0;
        }

        // update local player
        if (mapContains(players, localId))
        {
            players[localId]->update(windowInfo.DELTA_TIME, windowInfo);

            // packet senders in this conditional will be sent at a constant rate: 'packetSendRate'
            if (packetCanSend)
            {
                client_player_action_handleMove();
            }
        }

        if (!players.empty())
        {
            for (const auto &player : players)
            {
                player.second->render(worldCamera);
            }
        }

        std::string positionInfo = "PX: " +
                                   std::to_string(worldCamera->getTranslateX()) +
                                   "   PY: " + std::to_string(worldCamera->getTranslateY()) +
                                   "   /   ZX: " + std::to_string(worldCamera->getZoomX()) +
                                   "   ZY: " + std::to_string(worldCamera->getZoomY());
        drawText(renderWindow, positionInfo, ASSETS.FONTS.ALAGARD_FONT, sf::Vector2f(0, 720 - 30), 15);

        ++packetSendCounter;
        packetCanSend = false;
    }

    return CurrentScene::LobbyArena;
}