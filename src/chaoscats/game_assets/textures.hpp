#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../utils/logger.hpp"

struct TextureStorage
{
    sf::Texture PLAYER_HEAD_TEXTURE;
    sf::Texture PLAYER_HEAD_GORE1_TEXTURE;
    sf::Texture PLAYER_HEAD_GORE2_TEXTURE;
    sf::Texture PLAYER_HEAD_GORE3_TEXTURE;

    sf::Texture PLAYER_BODY_TEXTURE;
    sf::Texture PLAYER_BODY_GORE1_TEXTURE;
    sf::Texture PLAYER_BODY_GORE2_TEXTURE;
    sf::Texture PLAYER_BODY_GORE3_TEXTURE;

    sf::Texture PLAYER_LEG_TEXTURE;
    sf::Texture PLAYER_HAND_TEXTURE;

    sf::Texture WEAPON_SHOTGUN_TEXTURE;
    sf::Texture WEAPON_SHOTGUN_POINTS_TEXTURE;

    sf::Texture ANIMATED_SPRITE_TENNIS;

    TextureStorage()
    {
        loadTexture(PLAYER_HEAD_TEXTURE, "assets/textures/player/head/player_head.png");
        loadTexture(PLAYER_HEAD_GORE1_TEXTURE, "assets/textures/player/head/player_head_gore1.png");
        loadTexture(PLAYER_HEAD_GORE2_TEXTURE, "assets/textures/player/head/player_head_gore2.png");
        loadTexture(PLAYER_HEAD_GORE3_TEXTURE, "assets/textures/player/head/player_head_gore3.png");

        loadTexture(PLAYER_BODY_TEXTURE, "assets/textures/player/body/player_body.png");
        loadTexture(PLAYER_BODY_GORE1_TEXTURE, "assets/textures/player/body/player_body_gore1.png");
        loadTexture(PLAYER_BODY_GORE2_TEXTURE, "assets/textures/player/body/player_body_gore2.png");
        loadTexture(PLAYER_BODY_GORE3_TEXTURE, "assets/textures/player/body/player_body_gore3.png");

        loadTexture(PLAYER_LEG_TEXTURE, "assets/textures/player/leg/player_leg.png");
        loadTexture(PLAYER_HAND_TEXTURE, "assets/textures/player/hand/player_hand.png");

        loadTexture(WEAPON_SHOTGUN_TEXTURE, "assets/textures/weapons/shotgun/shotgun.png");
        loadTexture(WEAPON_SHOTGUN_POINTS_TEXTURE, "assets/textures/weapons/shotgun/shotgun_points.png");

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