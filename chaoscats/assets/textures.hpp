#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

struct TextureStorage
{
    sf::Texture TEST_PLAYER_TEXTURE;
    sf::Texture WEAPON_SHOTGUN_TEXTURE;

    TextureStorage()
    {
        loadTexture(TEST_PLAYER_TEXTURE, "assets/textures/player/test_player.png");
        loadTexture(WEAPON_SHOTGUN_TEXTURE, "assets/textures/weapons/shotgun/shotgun.png");
    };

private:
    void loadTexture(sf::Texture &texture, const std::string &path)
    {
        if (!texture.loadFromFile(path))
        {
            Logger::log(Logger::LogType::Error, "assets/textures", "Failed to load: " + path);
        }
    }
};