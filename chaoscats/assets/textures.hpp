#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

class TextureStorage
{
public:
    sf::Texture DOG_TEXTURE;
    sf::Texture TEST_WORLD_TEXTURE;
    sf::Texture TEST_PLAYER_TEXTURE;
    
    TextureStorage()
    {
        loadTexture(DOG_TEXTURE, "assets/dog.jpg");
        loadTexture(TEST_WORLD_TEXTURE, "assets/test_world.png");
        loadTexture(TEST_PLAYER_TEXTURE, "assets/test_player.png");
    };

    void loadTexture(sf::Texture &texture, const std::string &path)
    {
        if (!texture.loadFromFile(path))
        {
            std::cout << "[assets/textures.hpp] Failed to load: " << path << std::endl;
        }
    }
};