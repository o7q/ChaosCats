#include "matrix_math.hpp"

Mat2D<int> binaryMatrixFromTexture(sf::Texture &texture)
{
    sf::Vector2u textureSize = texture.getSize();
    sf::Image textureImage = texture.copyToImage();

    Mat2D<int> binaryMatrix(textureSize.y, textureSize.x);

    for (int y = 0; y < textureSize.y; ++y)
    {
        for (int x = 0; x < textureSize.x; ++x)
        {
            int alpha = textureImage.getPixel(x, y).a;
            int binaryValue = (alpha == 0) ? 0 : 1;

            binaryMatrix.set(y, x, binaryValue);
        }
    }

    return binaryMatrix;
}

Mat2D<int> hollowBinaryMatrix(Mat2D<int> &binaryMatrix)
{
    int rowSize = binaryMatrix.getRowSize();
    int colSize = binaryMatrix.getColSize();

    Mat2D<int> hollowMatrix(rowSize, colSize);

    for (int row = 0; row < rowSize; ++row)
    {
        for (int col = 0; col < colSize; ++col)
        {
            hollowMatrix.set(row, col, 0);

            if (binaryMatrix.get(row, col) == 1)
            {
                //    2
                // 1 [-] 3
                //    4

                bool isEdge = false;
                if (binaryMatrix.inBounds(row, col - 1))
                {
                    if (binaryMatrix.get(row, col - 1) == 0)
                    {
                        isEdge = true;
                    }
                }
                if (binaryMatrix.inBounds(row - 1, col))
                {
                    if (binaryMatrix.get(row - 1, col) == 0)
                    {
                        isEdge = true;
                    }
                }
                if (binaryMatrix.inBounds(row, col + 1))
                {
                    if (binaryMatrix.get(row, col + 1) == 0)
                    {
                        isEdge = true;
                    }
                }
                if (binaryMatrix.inBounds(row + 1, col))
                {
                    if (binaryMatrix.get(row + 1, col) == 0)
                    {
                        isEdge = true;
                    }
                }

                hollowMatrix.set(row, col, isEdge ? 1 : 0);
            }
        }
    }

    return hollowMatrix;
}