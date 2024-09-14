#pragma once

#include <vector>

#include "../particle.hpp"

#include "../../math/geometry/geometry_common.hpp"
#include "../../math/matrix/matrix.hpp"

void updateCollisionMesh(std::vector<Segment<int>> &collisionMesh, Mat2D<Particle> &particleWorld, PlayerCollisionType collisionType, int scale);

int getLookupIndex(int a, int b, int c, int d);
void appendLookupSegment(std::vector<Segment<int>> &collisionMesh, int index, int x, int y, int scale);