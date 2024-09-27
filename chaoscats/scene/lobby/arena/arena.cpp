#include "arena.hpp"

CurrentScene Scene_LobbyArena::run()
{
    if (init())
    {
        Logger::log(Logger::LogType::Info, "lobby-arena", "Initializing lobby arena");

        worldCamera = new WorldCamera();
        world = new World(renderWindow, 256, 144, 5);
    }

    if (update())
    {
        if (client.isConnectedToServer())
        {
            while (!client.incoming().empty())
            {
                auto message = client.incoming().pop_front().message;

                switch (message.header.id)
                {
                case GameMessages::Server2C_Validated:
                {
                    PlayerData new_pf;
                    net::Message<GameMessages> joinPacket;
                    joinPacket.header.id = GameMessages::Client2S_Register;
                    joinPacket << new_pf;
                    client.sendMessageToServer(joinPacket);
                }
                break;
                case GameMessages::Server2C_AssignID:
                {
                    message >> localId;
                    Logger::log(Logger::LogType::Info, "lobby-net", "Recieved new id from server: " + std::to_string(localId));
                }
                break;
                case GameMessages::Server2C_AddPlayer:
                {
                    PlayerData pf;
                    message >> pf;

                    Player *player = new Player(renderWindow, world, ASSETS.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));

                    player->setID(pf.id);
                    player->setPosition(pf.position.x, pf.position.y);

                    players.insert_or_assign(pf.id, player);

                    if (player->getPlayerData().id == localId)
                    {
                        validated = true;
                    }

                    Logger::log(Logger::LogType::Info, "lobby-net", "Player joined server: " + std::to_string(player->getPlayerData().id));
                }
                break;
                case GameMessages::Server2C_MovePlayer:
                {
                    PlayerData pf;
                    message >> pf;

                    if (players.find(pf.id) != players.end())
                    {
                        players[pf.id]->setPosition(pf.position.x, pf.position.y);
                    }
                }
                break;
                case GameMessages::Server2C_RemovePlayer:
                {
                    uint32_t id;
                    message >> id;

                    if (players.find(id) != players.end())
                    {
                        if (players[id] != nullptr)
                        {
                            delete players[id];
                        }

                        players.erase(id);
                    }
                }
                break;
                default:
                    break;
                }
            }
        }
        Particle drawParticle;
        drawParticle.color = sf::Color(255, 255, 0, 255);
        drawParticle.matterType = MatterType::Solid;
        drawParticle.physicsType = PhysicsType::Sand;
        drawParticle.playerCollisionType = PlayerCollisionType::Solid;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            drawParticle.color = sf::Color(0, 0, 0, 0);
            drawParticle.matterType = MatterType::None;
            drawParticle.physicsType = PhysicsType::None;
            drawParticle.playerCollisionType = PlayerCollisionType::None;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            drawParticle.color = sf::Color(0, 0, 255, 255);
            drawParticle.matterType = MatterType::Liquid;
            drawParticle.physicsType = PhysicsType::Water;
            drawParticle.playerCollisionType = PlayerCollisionType::Solid;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i localMouse = sf::Mouse::getPosition(renderWindow);
            sf::Vector2f mouseLoc;
            worldCamera->screenToWorld(localMouse.x, localMouse.y, mouseLoc.x, mouseLoc.y);
            world->drawParticles(mouseLoc.x / 5, mouseLoc.y / 5, drawParticle, 10);
        }

        world->update();
        world->render(worldCamera);

        if (validated)
        {
            players[localId]->update(windowInfo.DELTA_TIME, windowInfo);
            net::Message<GameMessages> movePacket;
            movePacket.header.id = GameMessages::Client2S_Move;
            movePacket << players[localId]->getPlayerData();
            client.sendMessageToServer(movePacket);
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
    }

    return CurrentScene::LobbyArena;
}