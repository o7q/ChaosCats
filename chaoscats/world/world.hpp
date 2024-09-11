#pragma once

#include <iostream>

#include "particle.hpp"
#include "../math/geometry/geometry_common.hpp"
#include "../math/matrix/matrix.hpp"
#include "../rendering/draw.hpp"

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

        worldPixels = new sf::Uint8[width * height * 4];
        worldTexture.create(width, height);

        if (!shader.loadFromFile("assets\\shaders\\particle\\shader.vert", "assets\\shaders\\particle\\shader.frag"))
        {
        }

        shader.setUniform("tex", worldTexture);
        shader.setUniform("texSize", sf::Vector2f(width, height));
        shader.setUniform("resolution", renderWindow.getView().getSize());

        quad.resize(4);
        quad.setPrimitiveType(sf::Quads);
        quad[0].position = sf::Vector2f(-1, -1);
        quad[1].position = sf::Vector2f(1, -1);
        quad[2].position = sf::Vector2f(1, 1);
        quad[3].position = sf::Vector2f(-1, 1);
        quad[0].texCoords = sf::Vector2f(0, 1);
        quad[1].texCoords = sf::Vector2f(1, 1);
        quad[2].texCoords = sf::Vector2f(1, 0);
        quad[3].texCoords = sf::Vector2f(0, 0);
    }
    ~World()
    {
        delete[] worldPixels;
    }

    void update()
    {
        particles_update();
        updateCollisionMesh(collisionMesh, particles, PlayerCollisionType::Solid, pixelSize);
    }

    void render()
    {
        renderWindow.draw(quad, &shader);
        for (Segment<int> segment : collisionMesh)
        {
            drawSegment(renderWindow, segment, sf::Color(0, 0, 255, 255));
        }
    }

    void setParticle(int x, int y, Particle particle)
    {
        particles.set(y, x, particle);

        int index = y * width + x;

        worldPixels[index + 0] = particle.color.r;
        worldPixels[index + 1] = particle.color.g;
        worldPixels[index + 2] = particle.color.b;
        worldPixels[index + 3] = particle.color.a;
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

    sf::Uint8 *worldPixels;
    sf::Texture worldTexture;

    sf::Shader shader;
    sf::VertexArray quad;

    std::vector<Segment<int>> collisionMesh;

    int width;
    int height;
    int pixelSize;

    void particles_update();
    void p_sand(Particle, int, int, BoundRules);
    void p_water(Particle, int, int, BoundRules);
};