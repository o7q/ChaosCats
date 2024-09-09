#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

class FontStorage
{
public:
    sf::Font ALAGARD_FONT;

    FontStorage()
    {
        loadFont(ALAGARD_FONT, "assets/alagard.ttf");
    };

    void loadFont(sf::Font &font, const std::string &path)
    {
        if (!font.loadFromFile(path))
        {
            std::cout << "[assets/fonts.hpp] Failed to load: " << path << std::endl;
        }
    }
};