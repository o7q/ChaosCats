#pragma once

#include <SFML/Graphics.hpp>

#include "matrix.hpp"

Mat2D<int> binaryMatrixFromTexture(sf::Texture &texture);
Mat2D<int> hollowBinaryMatrix(Mat2D<int> &binaryMatrix);