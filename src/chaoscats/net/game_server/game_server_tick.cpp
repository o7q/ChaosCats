#include "game_server.hpp"

void GameServer::start_tick()
{
    const std::chrono::milliseconds frameDuration(1000 / serverTickRate);
    while (keepTick.load())
    {
        std::chrono::high_resolution_clock::time_point tick_start = std::chrono::high_resolution_clock::now();

        std::chrono::high_resolution_clock::time_point start_delay_time = std::chrono::high_resolution_clock::now();
        tick();
        std::chrono::high_resolution_clock::time_point end_delay_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> tickTime = std::chrono::duration_cast<std::chrono::duration<float>>(end_delay_time - start_delay_time);

        // inject delay to force the server to run at at the set tickrate
        if (tickTime < frameDuration)
        {
            std::this_thread::sleep_for(frameDuration - tickTime);
        }

        std::chrono::high_resolution_clock::time_point tick_end = std::chrono::high_resolution_clock::now();

        serverGameInfo.time.refresh(tick_start, tick_end);
    }
}

void GameServer::tick()
{
    {
        std::lock_guard worldLock(worldMutex);
        world->update(serverGameInfo.time.DELTA_TIME);
    }

    updateProjectiles();
    updatePlayers();
}
