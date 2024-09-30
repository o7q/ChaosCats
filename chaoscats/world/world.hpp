#pragma once

#include <iostream>

#include "particle.hpp"
#include "camera.hpp"

#include "../math/geometry/geometry_common.hpp"
#include "../math/matrix/matrix.hpp"
#include "../rendering/draw.hpp"
#include "../math/random/random.hpp"

#include "collision/collision_mesh.hpp"

#include "../window/window_info.hpp"

#include "../assets/maps.hpp"

#include "../net/data/world.hpp"
#include "../net/messages.hpp"
#include "../net/framework/net.hpp"

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
    World(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, Map &map, int pixelSize = 10) : renderWindow(renderWindow), windowInfo(windowInfo), particles(map.height, map.width)
    {
        this->width = map.width;
        this->height = map.height;
        this->pixelSize = pixelSize;

        initWorldTexture();

        createWorldFromMap(map);
    }
    World(sf::RenderWindow &renderWindow, WindowInfo &windowInfo, int width, int height, int pixelSize = 10) : renderWindow(renderWindow), windowInfo(windowInfo), particles(height, width)
    {
        this->width = width;
        this->height = height;
        this->pixelSize = pixelSize;

        initWorldTexture();

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

    void createWorldFromMap(Map &map);
    void initWorldTexture();

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
            drawSegment(renderWindow, edge.segment, sf::Color(220, 220, 220, 255));
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

    void removeParticle(int x, int y)
    {
        setParticle(x, y, Particle());
    }

    net::Message<GameMessages> drawParticles(int loc_x, int loc_y, Particle particle, int radius = 5, DrawShape drawShape = DrawShape::Circle);

    Particle getParticle(int x, int y) const
    {
        if (particles.inBounds(y, x))
        {
            return particles.get(y, x);
        }

        return Particle();
    }

    Point<int> getWorldCoords(int worldX, int worldY) const
    {
        int x = worldX / pixelSize;
        int y = worldY / pixelSize;
        return Point<int>(x, y);
    }

    Particle getParticleFromWindowCoords(int x, int y) const
    {
        Point<int> worldCoords = getWorldCoords(x, y);
        if (particles.inBounds(worldCoords.y, worldCoords.x))
        {
            return particles.get(worldCoords.y, worldCoords.x);
        }

        return Particle();
    }

    int getPixelSize() const
    {
        return pixelSize;
    }

    void updateFromMessage(net::Message<GameMessages> &message)
    {
        size_t size;
        message >> size;

        for (size_t i = 0; i < size; ++i)
        {
            ParticleData particleData;
            message >> particleData;
            setParticle(particleData.worldX, particleData.worldY, particleData.particle);
        }
    }

    const std::vector<EdgeData> &getCollisionMesh() const
    {
        return collisionMesh;
    }

    Size<int> getSize() const
    {
        return Size<int>(width, height);
    }

private:
    sf::RenderWindow &renderWindow;
    WindowInfo &windowInfo;

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