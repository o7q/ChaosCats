#include "animated_sprite.hpp"

#include <cmath>
#include <utility>

#include "../utils/logger.hpp"
#include "../rendering/draw.hpp"

AnimatedSprite::AnimatedSprite(sf::Texture &frameSheet, unsigned int framesX, unsigned int framesY, unsigned int frameCount)
{
    this->framesX = framesX;
    this->framesY = framesY;
    this->frameCount = frameCount;

    sf::Vector2u textureSize = frameSheet.getSize();

    tileSize.x = textureSize.x / framesX;
    tileSize.y = textureSize.y / framesY;

    animatedSprite.setTexture(frameSheet);
    animatedSprite.setTextureRect(sf::IntRect(0, 0, tileSize.x, tileSize.y));
}

void AnimatedSprite::play(int framerate, float dt)
{
    if (frameCounter > (60.0f / dt) / static_cast<unsigned int>(framerate))
    {
        int x = currentFrame % framesX;
        int y = currentFrame / framesX;

        animatedSprite.setTextureRect(sf::IntRect(x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y));

        ++currentFrame;
        frameCounter = 0;
    }

    if (currentFrame > frameCount - 1)
    {
        currentFrame = 0;
        frameCounter = 0;
    }

    ++frameCounter;
}

void AnimatedSprite::render(
    sf::RenderWindow &renderWindow,
    const GameState::WindowState &windowState,
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const Camera *camera,
    const float &rotation,
    const sf::Vector2i &flipVector)
{
    drawSprite(renderWindow, windowState, animatedSprite, position, size, camera, rotation, flipVector);
}