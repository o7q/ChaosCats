#include "../arena.hpp"

// Server2C_SyncWorldStart
void Scene_LobbyArena::server_world_handleSyncWorldStart(net::Message<GameMessages> &message)
{
    Vector<int> worldSize;
    int pixelSize;
    int operations;
    message >> operations >> pixelSize >> worldSize;
    if (world != nullptr)
    {
        delete world;
        world = nullptr;
    }

    world = new World(worldSize.x, worldSize.y, pixelSize);
    world->set_s2c_buffer_count(operations);

    std::cout << worldSize.x << " " << worldSize.y << " " << pixelSize << std::endl;
}

// Server2C_SyncWorldChunk
void Scene_LobbyArena::server_world_handleSyncWorldChunk(net::Message<GameMessages> &message)
{
    if (world != nullptr)
    {
        int size;
        message >> size;
        for (int i = 0; i < size; ++i)
        {
            ParticleData pd;
            message >> pd;
            world->setParticle(pd.worldX, pd.worldY, pd.particle);
        }
    }

    world->check_s2c_progress();
}

// Server2C_ModifyWorld
void Scene_LobbyArena::server_world_action_handleModifyWorld(net::Message<GameMessages> &message)
{
    world->updateFromMessage(message);
}