#pragma once

#include <vector>

#include "../../math/geometry/geometry_common.hpp"
#include "../../math/matrix/matrix.hpp"

std::vector<Point2D<int>> generateCollisionMesh(Mat2D<int> &hollowBinaryMatrix, int scale);