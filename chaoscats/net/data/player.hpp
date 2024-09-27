#pragma once

#include <cstdint>

#include "../../math/geometry/geometry_common.hpp"

struct PlayerData
{
    uint32_t id = 0;

    Point<float> position;
};