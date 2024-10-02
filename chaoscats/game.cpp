#include "game.hpp"

#include "utils/logger.hpp"
#include "math/random/random.hpp"

GameWindow::GameWindow(const std::string &name, int width, int height, const std::vector<std::string> &netArgs) : netArgs(netArgs)
{
    renderWindow.create(sf::VideoMode(width, height), name);

    currentScene = CurrentScene::Lobby;

    int maxFps = 144;
    renderWindow.setFramerateLimit(maxFps);

    Random::seed();

    std::string fpsCountString = "";

    while (renderWindow.isOpen())
    {
        clock_t start = clock();

        sf::Event event;
        while (renderWindow.pollEvent(event))
        {
            sf::FloatRect visibleArea;
            switch (event.type)
            {
            case sf::Event::Closed:
                renderWindow.close();
                break;
            case sf::Event::GainedFocus:
                gameInfo.window.IS_FOCUSED = true;
                break;
            case sf::Event::LostFocus:
                gameInfo.window.IS_FOCUSED = false;
                break;
            default:
                break;
            }
        }

        renderWindow.clear(sf::Color::Black);
        update();

        // display on-screen framerate counter
        fpsCountString = "FPS: " + std::to_string(gameInfo.time.FRAMERATE) + "   DT: " + std::to_string(gameInfo.time.DELTA_TIME);
        drawText(renderWindow, fpsCountString, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(0, height - 15), 15);

        renderWindow.display();

        clock_t end = clock();

        gameInfo.time.refresh(start, end);
        gameInfo.window.refresh(renderWindow);
        gameInfo.input.refresh(renderWindow);
    }
}