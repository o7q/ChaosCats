#include "draw.hpp"

void drawSegment(
    sf::RenderWindow &renderWindow,
    const Segment<float> &segment,
    const Camera *camera,
    const sf::Color &color)
{
    sf::Vertex line[2];

    float resize_scale = calculateResizeScale(renderWindow.getSize());
    float aspect_scale = calculateAspectScale(renderWindow.getSize());
    line[0].position = transformPoint(sf::Vector2f(segment.p1.x, segment.p1.y), camera, renderWindow.getSize(), resize_scale, aspect_scale);
    line[0].color = color;
    line[1].position = transformPoint(sf::Vector2f(segment.p2.x, segment.p2.y), camera, renderWindow.getSize(), resize_scale, aspect_scale);
    line[1].color = color;

    renderWindow.draw(line, 2, sf::Lines);
}