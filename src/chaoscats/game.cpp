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
                gameState.window.IS_FOCUSED = true;
                break;
            case sf::Event::LostFocus:
                gameState.window.IS_FOCUSED = false;
                break;
            default:
                break;
            }
        }

        renderWindow.clear(sf::Color::Black);
        update();

        // display on-screen framerate counter
        fpsCountString = "FPS: " + std::to_string(gameState.time.FRAMERATE) +
                         " /   DT: " + std::to_string(gameState.time.DELTA_TIME) +
                         "   BDT: " + std::to_string(gameState.time.BASE_DELTA_TIME);
        drawText(renderWindow, gameState.window, fpsCountString, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(0, height - 15), 15, nullptr, sf::Color(0, 255, 0, 255));

        renderWindow.display();

        std::chrono::high_resolution_clock::time_point frame_end = std::chrono::high_resolution_clock::now();

        gameState.time.refresh(frame_start, frame_end);
        gameState.window.refresh(renderWindow);
        gameState.input.refresh(renderWindow);
    }
}