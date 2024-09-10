#include "main_menu.hpp"

#include "../../math/matrix/matrix.hpp"
#include "../../math/matrix/matrix_math.hpp"

#include <vector>

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "Main menu init" << std::endl;

        player = new Player(renderWindow, gameAssets.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));
        world = new World(renderWindow, 128, 80, 5);

        // Matrix2D<int> binaryMatrix = binaryMatrixFromTexture(gameAssets.TEXTURES.TEST_WORLD2_TEXTURE);
    }

    if (update())
    {
        // std::vector<Point<float>> points;

        // points.push_back(Point<float>(50, 600));
        // points.push_back(Point<float>(500, 700));
        // points.push_back(Point<float>(600, 600));
        // points.push_back(Point<float>(800, 400));
        // points.push_back(Point<float>(1000, 500));
        // points.push_back(Point<float>(1000, 100));
        // points.push_back(Point<float>(600, 100));
        // points.push_back(Point<float>(200, 0));

        // std::vector<Segment<float>> segments = segmentPathFromPoints(points);

        // for (Segment<float> segment : segments)
        // {
        //     drawSegment(renderWindow, segment);
        // }

        // player->update(segments, windowInfo);
        // player->draw();

        // Particle particle;
        // particle.physicsType = PhysicsType::Sand;
        // particle.matterType = MatterType::Solid;
        // particle.playerCollisionType = PlayerCollisionType::Solid;
        // particle.color = sf::Color(255, 255, 0, 255);

        // world->setParticle(0, 0, particle);
        // world->setParticle(0, 1, particle);
        // world->setParticle(0, 2, particle);

        // world->setParticle(0, 3, particle);
        // world->setParticle(1, 3, particle);
        // world->setParticle(2, 3, particle);
        // world->setParticle(3, 3, particle);
        // world->setParticle(4, 3, particle);
        // world->setParticle(1, 2, particle);
        // world->setParticle(2, 2, particle);
        // world->setParticle(3, 2, particle);
        // world->setParticle(2, 1, particle);

        // if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        // {
        //     sf::Vector2i localMouse = sf::Mouse::getPosition(renderWindow);
        //     world->drawParticles(localMouse.x / 5, localMouse.y / 5, particle, 2);
        // }
        // world->update();
        // world->render();

        Mat2D<int> binaryMatrix = binaryMatrixFromTexture(gameAssets.TEXTURES.TEST_WORLD_TEXTURE);

        Mat2D<int> hollowMatrix = hollowBinaryMatrix(binaryMatrix);

        std::vector<Point2D<int>> points = generateCollisionMesh(hollowMatrix, 1);

        for (Point2D<int> point : points)
        {
            std::cout << point.x << " " << point.y << std::endl;
        }

        system("pause");

        // std::vector<Segment<int>> segments = generateCollisionMesh(hollowMatrix);

        // for (int y = 0; y < binaryMatrix.getRowSize(); ++y)
        // {
        //     for (int x = 0; x < binaryMatrix.getColSize(); ++x)
        //     {
        //         if (binaryMatrix.get(y, x) == 1)
        //         {
        //             Rect<int> rect(Point2D<int>(x * 5, y * 5), Size2D<int>(5, 5));
        //             drawRect(renderWindow, rect, sf::Color(0, 0, 255, 255));
        //         }
        //     }
        // }

        // for (Segment<int> segment : segments)
        // {
        //     drawSegment(renderWindow, Segment<int>(Point2D<int>(segment.p1.x * 5, segment.p1.y * 5), Point2D<int>(segment.p2.x * 5, segment.p2.y * 5)), sf::Color(0, 255, 0, 255));
        // }
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}