#include "game.hpp"

#include "utils/logger.hpp"
#include "math/random.hpp"

GameWindow::GameWindow(const std::string &name, int width, int height, const std::vector<std::string> &netArgs) : netArgs(netArgs)
{
    renderWindow.create(sf::VideoMode(width, height), name, sf::Style::Default);

    currentScene = CurrentScene::Lobby;

    int maxFps = 144;
    renderWindow.setFramerateLimit(maxFps);

    Random::seed();

    std::string fpsCountString = "";

    while (renderWindow.isOpen())
    {
        std::chrono::high_resolution_clock::time_point frame_start = std::chrono::high_resolution_clock::now();

        sf::Event event;
        while (renderWindow.pollEvent(event))
        {
            sf::FloatRect visibleArea;
            switch (event.type)
            {
            case sf::Event::Closed:
                if (scene != nullptr)
                {
                    delete scene;
                    scene = nullptr;
                }
                renderWindow.close();
                break;
            case sf::Event::Resized:
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                renderWindow.setView(sf::View(visibleArea));
            }
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
        // drawText(renderWindow, fpsCountString, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(0, height - 15), 15);

        renderWindow.display();

        std::chrono::high_resolution_clock::time_point frame_end = std::chrono::high_resolution_clock::now();

        gameInfo.time.refresh(frame_start, frame_end);
        gameInfo.window.refresh(renderWindow);
        gameInfo.input.refresh(renderWindow);
    }
}