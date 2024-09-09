#include "game.hpp"

#include "math/matrix/matrix.hpp"

GameWindow::GameWindow(const std::string &name, int width, int height)
{
    renderWindow.create(sf::VideoMode(width, height), name);

    scene_mainMenu = new Scene_MainMenu(renderWindow, windowInfo, gameAssets);

    renderWindow.setFramerateLimit(60);

    while (renderWindow.isOpen())
    {
        clock_t start = clock();

        sf::Event event;
        while (renderWindow.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                renderWindow.close();
                break;
            case sf::Event::GainedFocus:
                windowInfo.IS_FOCUSED = true;
                break;
            case sf::Event::LostFocus:
                windowInfo.IS_FOCUSED = false;
                break;
            default:
                break;
            }
        }

        renderWindow.clear(sf::Color::Black);
        update();
        renderWindow.display();

        clock_t end = clock();
        windowInfo.DELTA_TIME = static_cast<float>(end - start) / CLOCKS_PER_SEC;
        windowInfo.FRAMERATE = std::round(1 / windowInfo.DELTA_TIME);
    }
}