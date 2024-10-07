#include "world.hpp"

#include "../utils/logger.hpp"

void World::initWorldTexture()
{
    worldPixels = new sf::Uint8[width * height * 4];
    worldTexture.create(width, height);

    if (!shader.loadFromFile("assets\\shaders\\particle\\shader.vert", "assets\\shaders\\particle\\shader.frag"))
    {
        Logger::log(Logger::LogType::Error, "initWorldTexture", "Failed to load particle shader");
    }

    shader.setUniform("tex", worldTexture);
    shader.setUniform("texSize", sf::Vector2f(width, height));

    quad.resize(4);
    quad.setPrimitiveType(sf::Quads);
    quad[0].position = sf::Vector2f(-1, -1);
    quad[1].position = sf::Vector2f(1, -1);
    quad[2].position = sf::Vector2f(1, 1);
    quad[3].position = sf::Vector2f(-1, 1);
    quad[0].texCoords = sf::Vector2f(0, 1);
    quad[1].texCoords = sf::Vector2f(1, 1);
    quad[2].texCoords = sf::Vector2f(1, 0);
    quad[3].texCoords = sf::Vector2f(0, 0);
}