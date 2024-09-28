#pragma once

#include <iostream>

#include "particle.hpp"
#include "../math/geometry/geometry_common.hpp"
#include "../math/matrix/matrix.hpp"
#include "../rendering/draw.hpp"
#include "../math/random/random.hpp"

#include "collision/collision_mesh.hpp"

#include "camera.hpp"

#include "../window/window_info.hpp"

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
    World(sf::RenderWindow &renderWindow, WindowInfo& windowInfo, int width, int height, int pixelSize = 10) : renderWindow(renderWindow), windowInfo(windowInfo), particles(height, width)
    {
        this->width = width;
        this->height = height;
        this->pixelSize = pixelSize;

        worldPixels = new sf::Uint8[width * height * 4];
        worldTexture.create(width, height);

        if (!shader.loadFromFile("assets\\shaders\\particle\\shader.vert", "assets\\shaders\\particle\\shader.frag"))
        {
            std::cerr << "Failed to load shader" << std::endl;
        }

        shader.setUniform("tex", worldTexture);
        shader.setUniform("texSize", sf::Vector2f(width, height));

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

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                removeParticle(x, y);
            }
        }
    }
    ~World()
    {
        delete[] worldPixels;
    }

    void update()
    {
        update_particles();
        updateCollisionMesh(collisionMesh, particles, PlayerCollisionType::Solid, pixelSize);
    }

    void render(WorldCamera *camera)
    {
        worldTexture.update(worldPixels);

        shader.setUniform("resolution", sf::Vector2f(windowInfo.WIDTH, windowInfo.HEIGHT));
        shader.setUniform("translation", sf::Vector2f(0, 0));
        shader.setUniform("zoom", 1.0f);

        renderWindow.draw(quad, &shader);

        for (EdgeData edge : collisionMesh)
        {
            drawSegment(renderWindow, Segment<float>(Point<float>(edge.segment.p1.x, edge.segment.p1.y * windowInfo.ASPECT), Point<float>(edge.segment.p2.x, edge.segment.p2.y * windowInfo.ASPECT)), sf::Color(0, 255, 255, 255));
        }
    }

    void setParticle(int x, int y, Particle particle)
    {
        particles.set(y, x, particle);

        int index = (y * width + x) * 4;

        worldPixels[index + 0] = particle.color.r;
        worldPixels[index + 1] = particle.color.g;
        worldPixels[index + 2] = particle.color.b;
        worldPixels[index + 3] = particle.color.a;
    }

    Particle getParticle(int x, int y)
    {
        if (particles.inBounds(y, x))
        {
            return particles.get(y, x);
        }

        return Particle();
    }

    Particle getParticleFromWorldCoords(int worldX, int worldY)
    {
        int y = worldY / pixelSize;
        int x = worldX / pixelSize;
        if (particles.inBounds(y, x))
        {
            return particles.get(y, x);
        }

        return Particle();
    }

    void removeParticle(int x, int y)
    {
        setParticle(x, y, Particle());
    }

    int getPixelSize()
    {
        return pixelSize;
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

    std::vector<EdgeData> &getCollisionMesh()
    {
        return collisionMesh;
    }

private:
    sf::RenderWindow &renderWindow;
    WindowInfo& windowInfo;

    Matrix<Particle> particles;

    std::vector<EdgeData> collisionMesh;

    sf::Shader shader;
    sf::VertexArray quad;
    sf::Uint8 *worldPixels;
    sf::Texture worldTexture;

    int width, height;
    int pixelSize;

    void update_particles();
    void p_sand(Particle, int, int, BoundRules);
    void p_water(Particle, int, int, BoundRules);
};