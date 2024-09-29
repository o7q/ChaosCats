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
                WINDOW_INFO.IS_FOCUSED = true;
                break;
            case sf::Event::LostFocus:
                WINDOW_INFO.IS_FOCUSED = false;
                break;
            default:
                break;
            }
        }

        handleInput();

        renderWindow.clear(sf::Color::Black);
        update();

        // display on-screen framerate counter
        fpsCountString = "FPS: " + std::to_string(WINDOW_INFO.FRAMERATE) + "   DT: " + std::to_string(WINDOW_INFO.DELTA_TIME);
        drawText(renderWindow, fpsCountString, ASSETS.FONTS.ALAGARD_FONT, sf::Vector2f(0, height - 15), 15);

        renderWindow.display();

        clock_t end = clock();
        WINDOW_INFO.FRAMERATE = std::round(1 / (static_cast<float>(end - start) / CLOCKS_PER_SEC));
        WINDOW_INFO.DELTA_TIME = 60.0f / WINDOW_INFO.FRAMERATE;
        sf::Vector2u windowSize = renderWindow.getSize();
        WINDOW_INFO.WIDTH = windowSize.x;
        WINDOW_INFO.HEIGHT = windowSize.y;
        WINDOW_INFO.ASPECT = windowSize.y / static_cast<float>(windowSize.x);
    }
}