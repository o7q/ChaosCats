#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

struct TextureStorage
{
    sf::Texture TEST_PLAYER_TEXTURE;

    TextureStorage()
    {
        loadTexture(TEST_PLAYER_TEXTURE, "assets/test_player.png");
    };

private:
    void loadTexture(sf::Texture &texture, const std::string &path)
    {
        if (!texture.loadFromFile(path))
        {
            std::cout << "[assets/textures.hpp] Failed to load: " << path << std::endl;
        }
    }
};