#pragma once

#include <SFML/Graphics.hpp>

#include "../camera/camera.hpp"
#include "../game_state/game_state.hpp"

class AnimatedSprite
{
public:
    AnimatedSprite(sf::Texture &frameSheet, unsigned int framesX, unsigned int framesY, unsigned int frameCount);

    void play(int framerate, int gameFramerate);
    void render(
        sf::RenderWindow &renderWindow,
        const GameState::WindowState &windowState,
        const sf::Vector2f &position,
        const sf::Vector2f &size,
        const Camera *camera = nullptr,
        const float &rotation = 0.0f,
        const sf::Vector2i &flipVector = sf::Vector2i(1, 1));

private:
    sf::Texture frameSheet;
    sf::Sprite animatedSprite;
    sf::Vector2u tileSize;

    unsigned int framesX = 0;
    unsigned int framesY = 0;
    unsigned int frameCount = 0;

    unsigned int currentFrame = 0;
    unsigned int frameCounter = 0;
};