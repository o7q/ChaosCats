#include "main_menu.hpp"

#include <vector>

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "Main menu init" << std::endl;

        player = new Player(renderWindow, gameAssets.TEXTURES.TEST_PLAYER_TEXTURE);
    }

    if (update())
    {
        std::vector<jmath::Point<float>> points;

        points.push_back(jmath::Point<float>(50, 600));
        points.push_back(jmath::Point<float>(500, 700));
        points.push_back(jmath::Point<float>(600, 600));
        points.push_back(jmath::Point<float>(800, 400));
        points.push_back(jmath::Point<float>(1000, 500));
        points.push_back(jmath::Point<float>(1150, 200));

        for (size_t i = 0; i < points.size(); i++)
        {
            size_t index = std::min(i, points.size() - 2);
            drawSegment(renderWindow, jmath::Segment<float>(points[index], points[index + 1]));
        }

        player->update();
        player->draw();
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}