#pragma once

#include <SFML/Graphics.hpp>

enum class PhysicsType
{
    Sand,
    Water,

    None
};

enum class MatterType
{
    Solid,
    Liquid,
    Gas,

    None
};

enum class PlayerCollisionType
{
    Solid,

    None
};

struct Particle
{
    PhysicsType physicsType = PhysicsType::None;
    MatterType matterType = MatterType::None;
    PlayerCollisionType playerCollisionType = PlayerCollisionType::None;

    sf::Color color = sf::Color(0, 0, 0, 0);

    bool passable()
    {
        return matterType == MatterType::Gas ||
               matterType == MatterType::Liquid ||
               matterType == MatterType::None;
    }
};