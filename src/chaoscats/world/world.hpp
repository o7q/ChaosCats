#pragma once

#include <iostream>

#include "particle.hpp"

#include "../math/types/types_common.hpp"
#include "../math/matrix.hpp"
#include "../rendering/draw.hpp"
#include "../math/random.hpp"

#include "collision_mesh/collision_mesh.hpp"

#include "../game_state/game_state.hpp"

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
    World(Map &map, int pixelSize = 10, bool serverWorld = false);
    World(int width, int height, int pixelSize = 10, bool serverWorld = false);
    ~World();

    void createWorldFromMap(Map &map);
    void initWorldTexture();

    void update(float dt, float base_dt);

    void render(sf::RenderWindow &renderWindow, GameState::WindowState &windowState, Camera *camera = nullptr);

    void setParticle(int x, int y, Particle particle);
    void removeParticle(int x, int y);

    net::Message<GameMessages> drawParticles(int loc_x, int loc_y, Particle particle, int radius = 5, DrawShape drawShape = DrawShape::Circle);

    Particle getParticle(int x, int y) const;
    Particle getParticleFromWindowCoords(int x, int y) const;

    Vector<int> getWorldCoords(int worldX, int worldY) const
    {
        int x = worldX / pixelSize;
        int y = worldY / pixelSize;
        return Vector<int>(x, y);
    }

    int getPixelSize() const
    {
        return pixelSize;
    }

    const std::vector<EdgeData> &getCollisionMesh() const
    {
        return collisionMesh;
    }

    Vector<int> getSize() const
    {
        return Vector<int>(width, height);
    }

    // net
    void updateFromMessage(net::Message<GameMessages> &message);

    // s2c: server to client
    // these functions are used when reconstructing the world from packets
    bool is_s2c_initialized() const;
    void check_s2c_progress();
    void set_s2c_buffer_count(int value);
    //

private:
    Matrix<Particle> particles;

    std::vector<EdgeData> collisionMesh;

    sf::Shader shader;
    sf::VertexArray quad;
    sf::Uint8 *worldPixels;
    sf::Texture worldTexture;

    int width, height;
    int pixelSize;

    bool s2c_initialized = false;
    int s2c_buffer_progress = 0;
    int s2c_buffer_count = 0;

    float update_counter = 0.0f;

    void update_particles();
    void p_sand(Particle, int, int, BoundRules);
    void p_water(Particle, int, int, BoundRules);

    bool serverWorld = false;
};