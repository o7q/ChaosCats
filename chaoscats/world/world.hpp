#pragma once

#include <iostream>

#include "particle.hpp"
#include "../math/geometry/geometry_common.hpp"
#include "../math/matrix/matrix.hpp"
#include "../rendering/draw.hpp"

#include "collision_mesh/binary_matrix.hpp"
#include "collision_mesh/collision_mesh.hpp"

struct BoundRules
{
    BoundRules(int x, int y, int mx, int my)
    {
        canUp = y > 0;
        canDown = y < my - 1;
        canLeft = x > 0;
        canRight = x < mx - 1;
    }

    bool canUp;
    bool canDown;
    bool canLeft;
    bool canRight;
};

enum class DrawShape
{
    Square,
    Circle
};

class World
{
public:
    World(sf::RenderWindow &renderWindow, int width, int height, int pixelSize = 10) : renderWindow(renderWindow), particles(height, width)
    {
        this->width = width;
        this->height = height;
        this->pixelSize = pixelSize;
    }

    void update()
    {
        particles_update();
        Mat2D<int> binmat = particleMatrixToHollowBinaryMatrix(particles, PlayerCollisionType::Solid);
        collisionMesh = generateCollisionMesh(binmat, pixelSize);
    }

    void render()
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Particle particle = particles.get(y, x);
                Rect<int> rect(Point2D<int>(x * pixelSize, y * pixelSize), Size2D<int>(pixelSize, pixelSize));
                drawRect(renderWindow, rect, sf::Color(particle.color));
            }
        }

        for (Segment<int> segment : collisionMesh)
        {
            drawSegment(renderWindow, segment, sf::Color(0, 0, 255, 255));
        }
    }

    void setParticle(int x, int y, Particle particle)
    {
        particles.set(y, x, particle);
    }

    void removeParticle(int x, int y)
    {
        particles.set(y, x, Particle());
    }

    void drawParticles(int loc_x, int loc_y, Particle particle, int radius = 5, DrawShape drawShape = DrawShape::Circle)
    {
        switch (drawShape)
        {
        case DrawShape::Square:
            for (int y = 0; y < radius; ++y)
            {
                for (int x = 0; x < radius; ++x)
                {
                    int x_clamped = std::clamp(loc_x + x - radius / 2, 0, width - 1);
                    int y_clamped = std::clamp(loc_y + y - radius / 2, 0, height - 1);

                    setParticle(x_clamped, y_clamped, particle);
                }
            }
            break;
        case DrawShape::Circle:
            for (int y = loc_y - radius; y <= loc_y + radius; ++y)
            {
                for (int x = loc_x - radius; x <= loc_x + radius; ++x)
                {
                    if ((x - loc_x) * (x - loc_x) + (y - loc_y) * (y - loc_y) <= radius * radius)
                    {
                        int x_clamped = std::clamp(x, 0, width - 1);
                        int y_clamped = std::clamp(y, 0, height - 1);

                        setParticle(x_clamped, y_clamped, particle);
                    }
                }
            }
            break;
        }
    }

private:
    sf::RenderWindow &renderWindow;
    Mat2D<Particle> particles;

    std::vector<Segment<int>> collisionMesh;

    int width;
    int height;
    int pixelSize;

    void particles_update();
    void p_sand(Particle, int, int, BoundRules);
};