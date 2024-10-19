#include "world.hpp"

void World::createWorldFromMap(Map &map)
{
    this->mapID = map.mapID;

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
            else if (material_mask_color == sf::Color(0, 0, 0, 255))
            {
                particle.physicsType = PhysicsType::None;
                particle.matterType = MatterType::Solid;
                particle.playerCollisionType = PlayerCollisionType::Solid;
                particle.playerMovementDampingType = PlayerMovementDampingType::Solid;
                particle.unbreakable = true;
            }
            // NO_GRAVITY
            else if (material_mask_color == sf::Color(32, 32, 32, 255))
            {
                particle.physicsType = PhysicsType::None;
                particle.matterType = MatterType::Solid;
                particle.playerCollisionType = PlayerCollisionType::Solid;
                particle.playerMovementDampingType = PlayerMovementDampingType::Solid;
            }
            // SAND
            else if (material_mask_color == sf::Color(255, 255, 0, 255))
            {
                particle.physicsType = PhysicsType::Sand;
                particle.matterType = MatterType::Solid;
                particle.playerCollisionType = PlayerCollisionType::Solid;
                particle.playerMovementDampingType = PlayerMovementDampingType::Solid;
            }
            // WATER
            else if (material_mask_color == sf::Color(0, 0, 255, 255))
            {
                particle.physicsType = PhysicsType::Water;
                particle.matterType = MatterType::Liquid;
                particle.playerCollisionType = PlayerCollisionType::Liquid;
                particle.playerMovementDampingType = PlayerMovementDampingType::Water;
            }

            setParticle(x, y, particle);
        }
    }
}