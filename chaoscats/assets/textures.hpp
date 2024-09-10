#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

class TextureStorage
{
public:
    sf::Texture TEST_PLAYER_TEXTURE;
    sf::Texture TEST_WORLD_TEXTURE;

    TextureStorage()
    {
        loadTexture(TEST_PLAYER_TEXTURE, "assets/test_player.png");
        loadTexture(TEST_WORLD_TEXTURE, "assets/test_world.png");
    };

    void loadTexture(sf::Texture &texture, const std::string &path)
    {
        if (!texture.loadFromFile(path))
        {
            std::cout << "[assets/textures.hpp] Failed to load: " << path << std::endl;
        }
    }
};