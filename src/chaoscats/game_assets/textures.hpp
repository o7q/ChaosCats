#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../utils/logger.hpp"

struct TextureStorage
{
    sf::Texture PLAYER_HEAD_DEFAULT;
    sf::Texture PLAYER_HEAD_DEFAULT_PREVIEW;
    sf::Texture PLAYER_HEAD_GORE1;
    sf::Texture PLAYER_HEAD_GORE2;
    sf::Texture PLAYER_HEAD_GORE3;

    sf::Texture PLAYER_BODY_DEFAULT;
    sf::Texture PLAYER_BODY_GORE1;
    sf::Texture PLAYER_BODY_GORE2;
    sf::Texture PLAYER_BODY_GORE3;

    sf::Texture PLAYER_LEG_DEFAULT;
    sf::Texture PLAYER_HAND_DEFAULT;

    sf::Texture WEAPON_SHOTGUN;
    sf::Texture WEAPON_SHOTGUN_POINTS;

    sf::Texture ANIMATED_SPRITE_TENNIS;

    TextureStorage()
    {
        loadTexture(PLAYER_HEAD_DEFAULT, "assets/textures/player/head/default/head_default.png");
        loadTexture(PLAYER_HEAD_DEFAULT_PREVIEW, "assets/textures/player/head/default/head_default_preview.png");
        loadTexture(PLAYER_HEAD_GORE1, "assets/textures/player/head/gore/head_gore1.png");
        loadTexture(PLAYER_HEAD_GORE2, "assets/textures/player/head/gore/head_gore2.png");
        loadTexture(PLAYER_HEAD_GORE3, "assets/textures/player/head/gore/head_gore3.png");

        loadTexture(PLAYER_BODY_DEFAULT, "assets/textures/player/body/body_default.png");
        loadTexture(PLAYER_BODY_GORE1, "assets/textures/player/body/gore/body_gore1.png");
        loadTexture(PLAYER_BODY_GORE2, "assets/textures/player/body/gore/body_gore2.png");
        loadTexture(PLAYER_BODY_GORE3, "assets/textures/player/body/gore/body_gore3.png");

        loadTexture(PLAYER_LEG_DEFAULT, "assets/textures/player/leg/leg_default.png");
        loadTexture(PLAYER_HAND_DEFAULT, "assets/textures/player/hand/hand_default.png");

        loadTexture(WEAPON_SHOTGUN, "assets/textures/weapons/shotgun/shotgun.png");
        loadTexture(WEAPON_SHOTGUN_POINTS, "assets/textures/weapons/shotgun/shotgun_points.png");

        loadTexture(ANIMATED_SPRITE_TENNIS, "assets/animations/tennis.png");
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