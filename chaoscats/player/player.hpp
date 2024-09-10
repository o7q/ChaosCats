#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>

#include "../math/geometry/geometry_common.hpp"
#include "../rendering/draw.hpp"
#include "../math/collision/collision.hpp"

class Player
{
public:
    Player(sf::RenderWindow &renderWindow, sf::Texture &playerTexture, sf::Vector2f size) : renderWindow(renderWindow), playerTexture(playerTexture)
    {
        playerSize = imageAspect(playerTexture, size);

        collisionBox = Rect<float>(Point2D<float>(0, 0), Size2D<float>(playerSize.x, playerSize.y));
    }

    void update(std::vector<Segment<float>> worldSegments, WindowInfo windowInfo)
    {
        float speed = 1.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            speed = 3.0f;
        }

        sf::Vector2f vel(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            vel.y -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            vel.y += speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            vel.x -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            vel.x += speed;
        }

        position.x += vel.x;
        position.y += vel.y;

        bool overlap = false;
        for (Segment<float> world_segment : worldSegments)
        {
            for (int shape = 0; shape < 2; ++shape)
            {
                for (int i = 0; i < 2; ++i)
                {
                    Point2D<float> worldSegment_midpoint((world_segment.p1.x + world_segment.p2.x) / 2.0f, (world_segment.p1.y + world_segment.p2.y) / 2.0f);
                    Point2D<float> worldSegment_point;

                    worldSegment_point = i == 0 ? world_segment.p1 : world_segment.p2;

                    Segment<float> worldSegmentDiagonal_diagonal(worldSegment_midpoint, worldSegment_point);

                    Point2D<float> displacement;

                    for (int j = 0; j < 4; ++j)
                    {
                        Point2D<float> collisionBox_start = Point2D<float>(position.x + collisionBox.points[j].x, position.y + collisionBox.points[j].y);
                        Point2D<float> collisionBox_end = Point2D<float>(position.x + collisionBox.points[(j + 1) % 4].x, position.y + collisionBox.points[(j + 1) % 4].y);

                        Segment<float> collisionBox_edge(collisionBox_start, collisionBox_end);

                        Point2D<float> line_r1s = worldSegment_midpoint;
                        Point2D<float> line_r1e = worldSegment_point;

                        Point2D<float> line_r2s = collisionBox_start;
                        Point2D<float> line_r2e = collisionBox_end;

                        // Standard "off the shelf" line segment intersection
                        float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
                        float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
                        float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;
                        // SS_OverlapData overlapData = segmentSegment(collisionBox_edge, worldSegmentDiagonal_diagonal);

                        if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
                        {
                            displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
                            displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
                            overlap = true;
                        }
                    }

                    position.x += displacement.x * (shape == 0 ? -1 : +1);
                    position.y += displacement.y * (shape == 0 ? -1 : +1);
                }
            }
        }

        collisionBox.position.x = position.x;
        collisionBox.position.y = position.y;
        drawRect(renderWindow, collisionBox, overlap ? sf::Color(255, 50, 50, 200) : sf::Color(50, 255, 50, 200), DrawMode::Outline);

        Rect<float> rect(Point2D<float>(position.x + collisionBox.size.w / 2, position.y + collisionBox.size.h / 2), Size2D<float>(20, 20));
        drawRect(renderWindow, rect, sf::Color(0, 0, 255, 255));
    }

    void draw()
    {
        drawTexture(renderWindow, playerTexture, position, playerSize);
    }

protected:
    sf::RenderWindow &renderWindow;
    sf::Texture &playerTexture;
    sf::Vector2f playerSize;

    Rect<float> collisionBox;

    sf::Vector2f position;
};