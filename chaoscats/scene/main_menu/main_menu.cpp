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

        // jmath::Matrix2D<int> binaryMatrix = binaryMatrixFromTexture(gameAssets.TEXTURES.TEST_WORLD2_TEXTURE);
    }

    if (update())
    {
        // std::vector<jmath::Point<float>> points;

        // points.push_back(jmath::Point<float>(50, 600));
        // points.push_back(jmath::Point<float>(500, 700));
        // points.push_back(jmath::Point<float>(600, 600));
        // points.push_back(jmath::Point<float>(800, 400));
        // points.push_back(jmath::Point<float>(1000, 500));
        // points.push_back(jmath::Point<float>(1000, 100));
        // points.push_back(jmath::Point<float>(600, 100));
        // points.push_back(jmath::Point<float>(200, 0));

        // std::vector<jmath::Segment<float>> segments = segmentPathFromPoints(points);

        // for (jmath::Segment<float> segment : segments)
        // {
        //     drawSegment(renderWindow, segment);
        // }

        // player->update(segments, windowInfo);
        // player->draw();

        jmath::Matrix2D<int> binaryMatrix = binaryMatrixFromTexture(gameAssets.TEXTURES.TEST_WORLD2_TEXTURE);

        jmath::Matrix2D<int> hollowMatrix = hollowBinaryMatrix(binaryMatrix);

        for (int y = 0; y < hollowMatrix.getRowSize(); ++y)
        {
            for (int x = 0; x < hollowMatrix.getColSize(); ++x)
            {
                if (hollowMatrix.get(y, x) == 1)
                {
                    jmath::Rectangle<int> rect(jmath::Point<int>(x * 2, y * 2), jmath::Size<int>(2, 2));
                    drawRectangle(renderWindow, rect);
                }
            }
        }
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}