#include "game_info.hpp"

#include <SFML/Graphics.hpp>

void GameInfo::WindowInfo::refresh(sf::RenderWindow &renderWindow)
{
    sf::Vector2u windowSize = renderWindow.getSize();

    WIDTH = windowSize.x;
    HEIGHT = windowSize.y;
    ASPECT = HEIGHT / static_cast<float>(WIDTH);
}