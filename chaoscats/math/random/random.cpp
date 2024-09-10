#include <iostream>

#include "random.hpp"

std::random_device Random::rd;
std::mt19937 Random::gen(Random::rd());

void Random::seed()
{
	gen.seed(rd());
}

int Random::genInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

float Random::genFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}