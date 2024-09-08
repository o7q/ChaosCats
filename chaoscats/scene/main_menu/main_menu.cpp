#include "main_menu.hpp"

#include "../../math/collision/collision_mesh.hpp"

#include <vector>

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "Main menu init" << std::endl;

        player = new Player(renderWindow, gameAssets.TEXTURES.TEST_WORLD_TEXTURE, sf::Vector2f(100, -1));
    }

    if (update())
    {
        std::vector<jmath::Point<float>> points;

        points.push_back(jmath::Point<float>(50, 600));
        points.push_back(jmath::Point<float>(500, 700));
        points.push_back(jmath::Point<float>(600, 600));
        points.push_back(jmath::Point<float>(800, 400));
        points.push_back(jmath::Point<float>(1000, 500));
        points.push_back(jmath::Point<float>(1000, 100));
        points.push_back(jmath::Point<float>(600, 100));
        points.push_back(jmath::Point<float>(200, 0));

        std::vector<jmath::Segment<float>> segments = segmentsFromPoints(points);

        for (jmath::Segment<float> segment : segments)
        {
            drawSegment(renderWindow, segment);
        }

        player->update(segments);
        player->draw();
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}