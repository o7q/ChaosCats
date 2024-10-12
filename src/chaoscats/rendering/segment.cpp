#include "draw.hpp"

void drawSegment(
    sf::RenderWindow &renderWindow,
    const GameInfo::WindowInfo &windowInfo,
    const Segment<float> &segment,
    const Camera *camera,
    const sf::Color &color)
{
    sf::Vertex line[2];

    line[0].position = point_worldToScreen(sf::Vector2f(segment.p1.x, segment.p1.y), camera, windowInfo);
    line[0].color = color;
    line[1].position = point_worldToScreen(sf::Vector2f(segment.p2.x, segment.p2.y), camera, windowInfo);
    line[1].color = color;

    renderWindow.draw(line, 2, sf::Lines);
}