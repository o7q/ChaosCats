#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../utils/logger.hpp"

struct TextureStorage
{
    sf::Texture PLAYER_TEXTURE;
    sf::Texture PLAYER_HAND_TEXTURE;
    sf::Texture WEAPON_SHOTGUN_TEXTURE;
    sf::Texture WEAPON_SHOTGUN_POINTS_TEXTURE;

    TextureStorage()
    {
        loadTexture(PLAYER_TEXTURE, "assets/textures/player/player.png");
        loadTexture(PLAYER_HAND_TEXTURE, "assets/textures/player/hand.png");
        loadTexture(WEAPON_SHOTGUN_TEXTURE, "assets/textures/weapons/shotgun/shotgun.png");
        loadTexture(WEAPON_SHOTGUN_POINTS_TEXTURE, "assets/textures/weapons/shotgun/shotgun_points.png");
    };

private:
    void loadTexture(sf::Texture &texture, const std::string &path)
    {
        Logger::log(Logger::LogType::Info, "loadTexture", "Loading: " + path);
        if (!texture.loadFromFile(path))
        {
            Logger::log(Logger::LogType::Error, "loadTexture", "Failed to load: " + path);
        }
    }
};