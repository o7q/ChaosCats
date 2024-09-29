#include "world.hpp"

void World::createWorldFromMap(Map &map)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            sf::Color map_color = map.map_color.getPixel(x, y);
            sf::Color material_mask_color = map.map_material_mask.getPixel(x, y);

            Particle particle;
            particle.color = map_color;

            // AIR
            if (material_mask_color == sf::Color(0, 0, 0, 0))
            {
            }
            // NO_GRAVITY (unbreakable)
            else if (material_mask_color == sf::Color(32, 32, 32, 255))
            {
                particle.physicsType = PhysicsType::None;
                particle.matterType = MatterType::Solid;
                particle.playerCollisionType = PlayerCollisionType::Solid;
                particle.unbreakable = true;
            }
            // NO_GRAVITY
            else if (material_mask_color == sf::Color(64, 64, 64, 255))
            {
                particle.physicsType = PhysicsType::None;
                particle.matterType = MatterType::Solid;
                particle.playerCollisionType = PlayerCollisionType::Solid;
            }
            // SAND
            else if (material_mask_color == sf::Color(255, 255, 0, 255))
            {
                particle.physicsType = PhysicsType::Sand;
                particle.matterType = MatterType::Solid;
                particle.playerCollisionType = PlayerCollisionType::Solid;
            }
            // WATER
            else if (material_mask_color == sf::Color(0, 0, 255, 255))
            {
                particle.physicsType = PhysicsType::Water;
                particle.matterType = MatterType::Liquid;
                particle.playerCollisionType = PlayerCollisionType::None;
            }

            setParticle(x, y, particle);
        }
    }
}

void World::initWorldTexture()
{
    worldPixels = new sf::Uint8[width * height * 4];
    worldTexture.create(width, height);

    if (!shader.loadFromFile("assets\\shaders\\particle\\shader.vert", "assets\\shaders\\particle\\shader.frag"))
    {
        std::cerr << "Failed to load shader" << std::endl;
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