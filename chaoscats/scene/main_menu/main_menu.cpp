#include <vector>

#include "main_menu.hpp"

#include "../../utils/logger.hpp"

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        Logger::log(Logger::LogType::Info, "mainmenu", "Initializing main menu");
    }

    if (update())
    {
        drawText(renderWindow, "hello", gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(100, 100), 40);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        return CurrentScene::Lobby;
    }

    return CurrentScene::MainMenu;
}