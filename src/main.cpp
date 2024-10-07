#include <iostream>
#include <vector>

#include "chaoscats/game.hpp"

int main(int argc, char *argv[])
{
	std::vector<std::string> arguments;
	for (int i = 1; i < argc; ++i)
	{
		arguments.push_back(argv[i]);
	}
	GameWindow gameWindow("Chaos Cats", 1280, 720, arguments);

	return 0;
}