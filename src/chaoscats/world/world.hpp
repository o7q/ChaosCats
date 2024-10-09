#pragma once

#include <iostream>

#include "particle.hpp"
#include "camera.hpp"

#include "../math/geometry/geometry_common.hpp"
#include "../math/matrix.hpp"
#include "../rendering/draw.hpp"
#include "../math/random.hpp"

#include "collision/collision_mesh.hpp"

#include "../game_info/game_info.hpp"

#include "../game_assets/maps.hpp"

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
    World(Map &map, int pixelSize = 10) : particles(map.height, map.width)
    {
        this->width = map.width;
        this->height = map.height;
        this->pixelSize = pixelSize;

        initWorldTexture();

        createWorldFromMap(map);
    }
    World(int width, int height, int pixelSize = 10) : particles(height, width)
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
        if (worldPixels != nullptr)
        {
            delete[] worldPixels;
            worldPixels = nullptr;
        }
    }

    void createWorldFromMap(Map &map);
    void initWorldTexture();

    void update(float dt)
    {
        for (int i = 0; i < static_cast<int>(std::ceil(dt)); ++i)
        {
            update_particles();
        }
        updateCollisionMesh(collisionMesh, particles, PlayerCollisionType::Solid, pixelSize);
    }

    void render(WorldCamera *camera, sf::RenderWindow &renderWindow, int width, int height)
    {
        worldTexture.update(worldPixels);

        shader.setUniform("resolution", sf::Vector2f(width, height));
        shader.setUniform("translation", sf::Vector2f(camera->getTranslateX(), camera->getTranslateY()));
        shader.setUniform("zoom", camera->getZoom());

        renderWindow.draw(quad, &shader);

        for (EdgeData edge : collisionMesh)
        {
            drawSegment(renderWindow, edge.segment, sf::Color(150, 150, 150, 180), camera);
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

    Vector<int> getWorldCoords(int worldX, int worldY) const
    {
        int x = worldX / pixelSize;
        int y = worldY / pixelSize;
        return Vector<int>(x, y);
    }

    Particle getParticleFromWindowCoords(int x, int y) const
    {
        Vector<int> worldCoords = getWorldCoords(x, y);
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

    bool isInitialized() const
    {
        return initialized;
    }

    const std::vector<EdgeData> &getCollisionMesh() const
    {
        return collisionMesh;
    }

    Vector<int> getSize() const
    {
        return Vector<int>(width, height);
    }

    void check_s2c_progress()
    {
        ++s2c_buffer_progress;
        if (s2c_buffer_progress == s2c_buffer_count)
        {
            initialized = true;
        }
    }

    void set_s2c_buffer_count(int value)
    {
        this->s2c_buffer_count = value;
    }

private:
    Matrix<Particle> particles;

    std::vector<EdgeData> collisionMesh;

    sf::Shader shader;
    sf::VertexArray quad;
    sf::Uint8 *worldPixels;
    sf::Texture worldTexture;

    int width, height;
    int pixelSize;

    bool initialized = false;

    int s2c_buffer_progress = 0;
    int s2c_buffer_count = 0;

    void update_particles();
    void p_sand(Particle, int, int, BoundRules);
    void p_water(Particle, int, int, BoundRules);
};