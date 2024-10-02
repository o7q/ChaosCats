#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../utils/logger.hpp"

struct FontStorage
{
    sf::Font ALAGARD_FONT;

    FontStorage()
    {
        loadFont(ALAGARD_FONT, "assets/fonts/alagard.ttf");
    };

private:
    void loadFont(sf::Font &font, const std::string &path)
    {
        if (!font.loadFromFile(path))
        {
            Logger::log(Logger::LogType::Error, "loadFont", "Failed to load: " + path);
        }
    }
};