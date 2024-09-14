#include "main_menu.hpp"

#include "../../math/matrix/matrix.hpp"

#include <vector>

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "Main menu init" << std::endl;

        worldCamera = new WorldCamera();
        world = new World(renderWindow, 256, 128, 5);
        player = new Player(renderWindow, world, gameAssets.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));
    }

    if (update())
    {
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
        //     drawParticle.color = sf::Color(0, 0, 255, 255);
        //     drawParticle.matterType = MatterType::Liquid;
        //     drawParticle.physicsType = PhysicsType::Water;
        //     drawParticle.playerCollisionType = PlayerCollisionType::Solid;
        // }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        // {
        //     worldCamera->translate(-1, 0);
        // }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        // {
        //     worldCamera->translate(1, 0);
        // }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        // {
        //     worldCamera->translate(0, -1);
        // }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        // {
        //     worldCamera->translate(0, 1);
        // }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        // {
        //     worldCamera->zoom(1.001, 1.001);
        // }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        // {
        //     worldCamera->zoom(0.999, 0.999);
        // }

        // if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        // {
        //     sf::Vector2i localMouse = sf::Mouse::getPosition(renderWindow);
        //     sf::Vector2f mouseLoc;
        //     worldCamera->screenToWorld(localMouse.x, localMouse.y, mouseLoc.x, mouseLoc.y);
        //     world->drawParticles(mouseLoc.x / 5, mouseLoc.y / 5, drawParticle, 8);
        // }

        // world->update();
        // world->render(worldCamera);

        std::vector<Point2D<float>> points;

        points.push_back(Point2D<float>(50, 600));
        points.push_back(Point2D<float>(500, 700));
        points.push_back(Point2D<float>(600, 600));
        points.push_back(Point2D<float>(800, 400));
        points.push_back(Point2D<float>(1000, 500));
        points.push_back(Point2D<float>(1000, 100));
        points.push_back(Point2D<float>(600, 100));
        points.push_back(Point2D<float>(200, 0));

        std::vector<Segment<float>> segments = segmentPathFromPoints(points);

        for (Segment<float> segment : segments)
        {
            drawSegment(renderWindow, segment);
        }

        player->update(windowInfo.DELTA_TIME, segments);
        player->render(worldCamera);

        std::string positionInfo = "PX: " +
                                   std::to_string(worldCamera->getTranslateX()) +
                                   "   PY: " + std::to_string(worldCamera->getTranslateY()) +
                                   "   /   ZX: " + std::to_string(worldCamera->getZoomX()) +
                                   "   ZY: " + std::to_string(worldCamera->getZoomY());
        drawText(renderWindow, positionInfo, gameAssets.FONTS.ALAGARD_FONT, sf::Vector2f(0, 720 - 30), 15);
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}