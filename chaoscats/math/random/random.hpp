#pragma once

#include <random>

class Random
{
public:
    static void seed();
    static int genInt(int min, int max);
    static float genFloat(float min, float max);

private:
    static std::random_device rd;
    static std::mt19937 gen;
};