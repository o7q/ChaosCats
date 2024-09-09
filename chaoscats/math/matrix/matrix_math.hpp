#pragma once

#include <SFML/Graphics.hpp>

#include "matrix.hpp"

jmath::Matrix2D<int> binaryMatrixFromTexture(sf::Texture &texture);
jmath::Matrix2D<int> hollowBinaryMatrix(jmath::Matrix2D<int> &binaryMatrix);