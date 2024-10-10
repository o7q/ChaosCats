#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../utils/logger.hpp"
#include "../rendering/draw.hpp"

class AnimatedSprite
{
public:
    AnimatedSprite(sf::Texture &frameSheet, unsigned int framesX, unsigned int framesY, unsigned int frameCount)
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

    void play(int framerate, int gameFramerate)
    {
        if (frameCounter > static_cast<unsigned int>(gameFramerate / framerate))
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

    void render(sf::RenderWindow &renderWindow, sf::Vector2f position, sf::Vector2f size, float rotation = 0.0f, sf::Vector2i flipVector = sf::Vector2i(1, 1), WorldCamera *camera = nullptr)
    {
        sf::Sprite renderSprite = animatedSprite;
        renderSprite.setRotation(rotation);

        sf::Vector2f newSize = imageAspect(tileSize, size);

        renderSprite.setOrigin(tileSize.x / 2, tileSize.y / 2);
        renderSprite.setPosition(position.x, position.y);
        renderSprite.setScale(newSize.x / tileSize.x * flipVector.x, newSize.y / tileSize.y * flipVector.y);

        renderWindow.draw(renderSprite);
    }

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