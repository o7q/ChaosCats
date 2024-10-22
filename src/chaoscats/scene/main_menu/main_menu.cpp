#include <vector>

#include "main_menu.hpp"

#include "../../utils/logger.hpp"
#include "../../controls/button.hpp"

Button *button = nullptr;

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        Logger::log(Logger::LogType::Info, "mainmenu", "Initializing main menu");
        button = new Button(100, 100, 200, 100, "hello", gameAssets.fonts.ALAGARD_FONT);
    }

    if (update())
    {
        button->update(gameState);

        if (button->hasMouseClicked())
        {
            std::cout << "click" << std::endl;
        }

        button->render(renderWindow, gameState);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H) && gameState.window.IS_FOCUSED)
    {
        return CurrentScene::Lobby;
    }

    return CurrentScene::MainMenu;
}