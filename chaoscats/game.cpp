#include "game.hpp"

#include "math/matrix.hpp"

GameWindow::GameWindow(const std::string &name, int width, int height)
{
    renderWindow.create(sf::VideoMode(width, height), name);

    scene_mainMenu = new Scene_MainMenu(renderWindow, gameAssets);

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
                windowInfo.isFocused = true;
                break;
            case sf::Event::LostFocus:
                windowInfo.isFocused = false;
                break;
            default:
                break;
            }
        }

        renderWindow.clear(sf::Color::Black);
        update();
        renderWindow.display();

        clock_t end = clock();
        double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
        FRAMERATE = static_cast<int>(std::round(1 / elapsed_time));
    }
}