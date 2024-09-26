#include <vector>

#include "main_menu.hpp"

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "MainMenu init" << std::endl;
    }

    if (update())
    {
        drawText(renderWindow, "hello", ASSETS.FONTS.ALAGARD_FONT, sf::Vector2f(100, 100), 40);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        return CurrentScene::Lobby;
    }

    return CurrentScene::MainMenu;
}