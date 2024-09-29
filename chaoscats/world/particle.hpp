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
    bool unbreakable = false;

    sf::Color color = sf::Color(0, 0, 0, 0);

    bool isPassable()
    {
        return matterType == MatterType::Gas ||
               matterType == MatterType::Liquid ||
               matterType == MatterType::None;
    }

    bool isSolid()
    {
        return matterType == MatterType::Solid;
    }

    bool isLiquid()
    {
        return matterType == MatterType::Liquid;
    }

    bool isAir()
    {
        return matterType == MatterType::Gas ||
               matterType == MatterType::None;
    }
};