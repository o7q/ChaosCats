#include "world.hpp"

void World::check_s2c_progress()
{
    ++s2c_buffer_progress;
    if (s2c_buffer_progress == s2c_buffer_count)
    {
        s2c_initialized = true;
    }
}

void World::set_s2c_buffer_count(int value)
{
    this->s2c_buffer_count = value;
}

bool World::is_s2c_initialized() const
{
    return s2c_initialized;
}

void World::updateFromMessage(net::Message<GameMessages> &message)
{
    size_t size;
    message >> size;

    for (size_t i = 0; i < size; ++i)
    {
        ParticleData particleData;
        message >> particleData;
        setParticle(particleData.worldX, particleData.worldY, particleData.particle);
    }
}