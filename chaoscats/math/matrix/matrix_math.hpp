#pragma once

#include <SFML/Graphics.hpp>

#include "matrix.hpp"

Matrix2D<int> binaryMatrixFromTexture(sf::Texture &texture);
Matrix2D<int> hollowBinaryMatrix(Matrix2D<int> &binaryMatrix);