#include "main_menu.hpp"

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "Main menu init" << std::endl;
    }

    if (update())
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(renderWindow);
        // jmath::geo::Segment<float> segment {100, 1000, mousePosition.x, mousePosition.y};
        jmath::geo::Segment<float> segment2 {500, 500, 1000, 700};
        // bool intersect = jmath::col::segmentSegment(segment, segment2);
        // drawSegment(renderWindow, segment, sf::Color(100, 25, 255, 255));

        jmath::geo::Rectangle<float> rectangle(jmath::geo::Point<float>{mousePosition.x, mousePosition.y}, jmath::geo::Size<float>{100, 100});
        bool intersect = jmath::col::segmentRectangle(segment2, rectangle);
        drawSegment(renderWindow, segment2);
        drawRectangle(renderWindow, rectangle, intersect ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255));
        // drawTexture(renderWindow, gameAssets.TEXTURES.DOG_TEXTURE, rectangle);
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}