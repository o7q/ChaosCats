#include "game.hpp"
#include "logger/logger.hpp"

#include "net/packets/player.hpp"

#include "math/matrix/matrix.hpp"

GameWindow::GameWindow(const std::string &name, int width, int height, const std::vector<std::string> &netArgs) : netArgs(netArgs)
{
    renderWindow.create(sf::VideoMode(width, height), name);

    currentScene = CurrentScene::Lobby;

    int maxFps = 144;
    renderWindow.setFramerateLimit(maxFps);

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
                windowInfo.IS_FOCUSED = true;
                break;
            case sf::Event::LostFocus:
                windowInfo.IS_FOCUSED = false;
                break;
            case sf::Event::Resized:
                visibleArea = sf::FloatRect(0.0f, 0.0f, event.size.width, event.size.height);
                renderWindow.setView(sf::View(visibleArea));
                break;
            default:
                break;
            }
        }

        renderWindow.clear(sf::Color::Black);
        update();

        // display on-screen framerate counter
        fpsCountString = "FPS: " + std::to_string(windowInfo.FRAMERATE) + "   DT: " + std::to_string(windowInfo.DELTA_TIME);
        drawText(renderWindow, fpsCountString, ASSETS.FONTS.ALAGARD_FONT, sf::Vector2f(0, height - 15), 15);

        renderWindow.display();

        clock_t end = clock();
        windowInfo.FRAMERATE = std::round(1 / (static_cast<float>(end - start) / CLOCKS_PER_SEC));
        windowInfo.DELTA_TIME = 60.0f / windowInfo.FRAMERATE;
    }
}