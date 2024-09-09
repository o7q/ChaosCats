#include "main_menu.hpp"

#include "../../math/collision/collision_mesh.hpp"
#include "../../math/matrix/matrix.hpp"
#include "../../math/matrix/matrix_math.hpp"

#include <vector>

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "Main menu init" << std::endl;

        player = new Player(renderWindow, gameAssets.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));

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

        Matrix2D<int> binaryMatrix = binaryMatrixFromTexture(gameAssets.TEXTURES.TEST_WORLD3_TEXTURE);

        Matrix2D<int> hollowMatrix = hollowBinaryMatrix(binaryMatrix);

        std::vector<Point<int>> points = pointsFromHollowBinaryMatrix(hollowMatrix);

        for (Point<int> point : points)
        {
            std::cout << point.x << " " << point.y << std::endl;
        }

        // std::vector<Segment<int>> collisionMesh = segmentPathFromPoints(points);

        // for (Segment<int> segment : collisionMesh)
        // {
        //     drawSegment(renderWindow, segment);
        // }

        // for (int y = 0; y < hollowMatrix.getRowSize(); ++y)
        // {
        //     for (int x = 0; x < hollowMatrix.getColSize(); ++x)
        //     {
        //         if (hollowMatrix.get(y, x) == 1)
        //         {
        //             Rectangle<int> rect(Point<int>(x * 2, y * 2), Size<int>(2, 2));
        //             drawRectangle(renderWindow, rect);
        //         }
        //     }
        // }
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}