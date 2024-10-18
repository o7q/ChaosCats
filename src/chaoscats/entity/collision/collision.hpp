#pragma once

#include "../../math/types/types_common.hpp"
#include "../../world/collision_mesh/collision_mesh.hpp"

void solveWorldCollision(
    const Segment<float> &collisionSegment,
    const EdgeData &edge,
    const Rect<float> &collisionBox,
    Vector<float> &position,
    Vector<float> &velocity,
    const float &dt);