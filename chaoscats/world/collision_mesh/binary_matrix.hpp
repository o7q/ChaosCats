#pragma once

#include "../../math/matrix/matrix.hpp"
#include "../particle.hpp"

Mat2D<int> particleMatrixToHollowBinaryMatrix(Mat2D<Particle> &particleMatrix, PlayerCollisionType collisionType);