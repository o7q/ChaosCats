#pragma once

#include <vector>

#include "collision/collision.hpp"
#include "../world/world.hpp"
#include "../math/types/vector.hpp"
#include "../game_state/game_state.hpp"

class Entity
{
public:
    Entity(const GameState &gameState, const World *world = nullptr) : gameState(gameState), world(world)
    {
    }
    virtual ~Entity()
    {
    }

    virtual void setPosition(Vector<float> position)
    {
        this->position = position;
        updateChildPositions();
    }

    virtual void setVelocity(Vector<float> velocity)
    {
        this->velocity = velocity;
    }

    virtual Vector<float> getPosition() const
    {
        return position;
    }

    virtual Vector<float> getVelocity() const
    {
        return velocity;
    }

    virtual Vector<float> getSize() const
    {
        return boundingBox.size;
    }

    virtual void moveUpdate(bool checkWorldCollisions);

protected:
    virtual void updateChildPositions()
    {
        boundingBox.setPosition(position.x, position.y);

        Vector<float> bottomCollisionPoint(
            boundingBox.getPosition().x,
            boundingBox.getPosition().y + boundingBox.half_size.y);

        Vector<float> topCollisionPoint(
            boundingBox.getPosition().x,
            boundingBox.getPosition().y - boundingBox.half_size.y);

        collisionSegment.p1 = topCollisionPoint;
        collisionSegment.p2 = bottomCollisionPoint;
    }

    const GameState &gameState;
    const World *world = nullptr;

    Vector<float> position;
    Vector<float> velocity;

    Segment<float> collisionSegment;

    Rect<float> boundingBox;
};