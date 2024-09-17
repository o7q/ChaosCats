#include "game.hpp"

#include "math/matrix/matrix.hpp"

GameWindow::GameWindow(const std::string &name, int width, int height)
{
    renderWindow.create(sf::VideoMode(width, height), name);

    scene_mainMenu = new Scene_MainMenu(renderWindow, windowInfo, gameAssets);

    int maxFps = 60;
    renderWindow.setFramerateLimit(maxFps);

    std::string fpsCountString = "";
    float fpsCountTimer = 0.0f;

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
                visibleArea = sf::FloatRect(0.f, 0.f, event.size.width, event.size.height);
                renderWindow.setView(sf::View(visibleArea));
                break;
            default:
                break;
            }
        }

        renderWindow.clear(sf::Color::Black);
        update();

        // display on-screen framerate counter
        if (fpsCountTimer >= 0.5f)
        {
            fpsCountTimer = 0.0f;
            fpsCountString = "FPS: " + std::to_string(windowInfo.FRAMERATE) + "   DT: " + std::to_string(windowInfo.DELTA_TIME);
        }

        drawText(renderWindow, fpsCountString, gameAssets.FONTS.ALAGARD_FONT, sf::Vector2f(0, height - 15), 15);

        renderWindow.display();

        clock_t end = clock();
        windowInfo.FRAMERATE = std::round(1 / (static_cast<float>(end - start) / CLOCKS_PER_SEC));
        windowInfo.DELTA_TIME = 60.0f / windowInfo.FRAMERATE;

        fpsCountTimer += windowInfo.DELTA_TIME;
    }
}