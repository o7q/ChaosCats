#pragma once

#include <vector>

#include "../../math/geometry/geometry_common.hpp"
#include "../../math/matrix/matrix.hpp"

std::vector<Segment<int>> generateCollisionMesh(Mat2D<int> &hollowBinaryMatrix, int scale);