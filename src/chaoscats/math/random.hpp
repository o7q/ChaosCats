#pragma once

#include <iostream>
#include <cstdint>
#include <random>

#include "../utils/logger.hpp"

namespace Random
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    inline void seed()
    {
        Logger::log(Logger::LogType::Info, "random", "Seeding random");
        gen.seed(rd());
    }

    inline int genInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    inline float genFloat(float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    class LCG
    {
    private:
        // lcg values, found from: https://en.wikipedia.org/wiki/Linear_congruential_generator
        const uint64_t a = 1664525;    // multiplier a
        const uint64_t c = 1013904223; // increment c
        const uint64_t m = 4294967296; // 2^32
        uint64_t seed;

    public:
        LCG(uint64_t seed) : seed(seed) {}

        uint32_t next()
        {
            seed = (a * seed + c) % m;
            return static_cast<uint32_t>(seed);
        }

        uint32_t next_in_range(uint32_t min, uint32_t max)
        {
            uint32_t value = next();
            return min + (value % (max - min + 1));
        }
    };
};