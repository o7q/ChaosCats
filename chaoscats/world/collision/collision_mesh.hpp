#pragma once

#include <vector>

#include "../particle.hpp"

#include "../../math/geometry/geometry_common.hpp"
#include "../../math/matrix/matrix.hpp"

enum class SlopeType
{
    UpHill,
    DownHill,
    Vertical,
    Horizontal
};

struct EdgeData
{
    Segment<float> segment;
    int worldX = 0;
    int worldY = 0;
    
    SlopeType slopeType = SlopeType::Horizontal;
    float slope = 0;
    float b = 0;
};

void updateCollisionMesh(std::vector<EdgeData> &collisionMesh, Matrix<Particle> &particleWorld, PlayerCollisionType collisionType, int scale);

int getLookupIndex(int a, int b, int c, int d);
void appendLookupSegment(std::vector<EdgeData> &collisionMesh, int index, int x, int y, int scale);