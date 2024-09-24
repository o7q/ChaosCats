#include <vector>

#include <thread>

#include "arena.hpp"

void Scene_Arena::runServer()
{
    while (keepServer)
    {
        gameServer->update(-1, true);
    }
}

CurrentScene Scene_Arena::run()
{
    if (init())
    {
        std::cout << "Arena init" << std::endl;

        worldCamera = new WorldCamera();
        world = new World(renderWindow, 256, 144, 5);

        // player = new Player(renderWindow, world, gameAssets.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));

        std::string address;
        uint16_t port;

        // if a server, host the server, then join on the localhost
        if (netArgs[0] == "host")
        {
            std::cout << "mode: serverhost" << std::endl;

            address = "";
            port = std::stoi(netArgs[1]);

            gameServer = new GameServer(port);
            gameServer->start();

            keepServer = true;

            serverThread = std::thread(&Scene_Arena::runServer, this);

            gameClient.connect("127.0.0.1", port);
        }
        // if a client, join the server using the address and port specified
        else if (netArgs[0] == "join")
        {
            std::cout << "mode: clientjoin" << std::endl;

            address = netArgs[1];
            port = std::stoi(netArgs[2]);

            gameClient.connect(address, port);
        }

        // player->pickupWeapon(gameAssets.TEXTURES.WEAPON_SHOTGUN_TEXTURE);
    }

    if (update())
    {
        if (gameClient.isConnectedToServer())
        {
            while (!gameClient.incoming().empty())
            {
                auto message = gameClient.incoming().pop_front().message;

                switch (message.header.id)
                {
                case GamePackets::Server2C_Validated:
                {
                    PlayerFramework new_pf;
                    jnet::Message<GamePackets> joinPacket;
                    joinPacket.header.id = GamePackets::Client2S_Register;
                    joinPacket << new_pf;
                    gameClient.sendMessageToServer(joinPacket);
                }
                break;
                case GamePackets::Server2C_AssignID:
                {
                    message >> localId;
                    std::cout << "Recieved new id from server: " << localId << std::endl;
                }
                break;
                case GamePackets::Server2C_AddPlayer:
                {
                    // PlayerFramework pf;
                    // message >> pf;

                    // Player *player = new Player(renderWindow, world, gameAssets.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));

                    // player->pf.id = pf.id;
                    // player->pf.position = pf.position;

                    // players.insert_or_assign(pf.id, player);

                    // std::cout << pf.id << std::endl;

                    // if (player->pf.id == localId)
                    // {
                    //     validated = true;
                    // }

                    // std::cout << "Player joined server: " << player->pf.id << std::endl;
                }
                break;
                case GamePackets::Server2C_MovePlayer:
                {
                    PlayerFramework pf;
                    message >> pf;

                    players[pf.id]->pf.position = pf.position;
                }
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
            players[localId]->update(windowInfo.DELTA_TIME);
            jnet::Message<GamePackets> movePacket;
            movePacket.header.id = GamePackets::Client2S_Move;
            movePacket << players[localId]->pf;
            gameClient.sendMessageToServer(movePacket);
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
        drawText(renderWindow, positionInfo, gameAssets.FONTS.ALAGARD_FONT, sf::Vector2f(0, 720 - 30), 15);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
    {
        return CurrentScene::MainMenu;
    }

    return CurrentScene::Arena;
}