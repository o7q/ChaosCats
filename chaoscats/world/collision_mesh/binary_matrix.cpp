#include "binary_matrix.hpp"

Mat2D<int> particleMatrixToHollowBinaryMatrix(Mat2D<Particle> &particleMatrix, PlayerCollisionType collisionType)
{
    int width = particleMatrix.getColSize();
    int height = particleMatrix.getRowSize();

    Mat2D<int> binaryCollisionMatrix(height, width);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (particleMatrix.get(y, x).playerCollisionType == collisionType)
            {
                //    2
                // 1 [-] 3
                //    4

                bool isEdge = false;
                if (particleMatrix.inBounds(y, x - 1))
                {
                    if (particleMatrix.get(y, x - 1).playerCollisionType != collisionType)
                    {
                        isEdge = true;
                    }
                }
                if (particleMatrix.inBounds(y - 1, x))
                {
                    if (particleMatrix.get(y - 1, x).playerCollisionType != collisionType)
                    {
                        isEdge = true;
                    }
                }
                if (particleMatrix.inBounds(y, x + 1))
                {
                    if (particleMatrix.get(y, x + 1).playerCollisionType != collisionType)
                    {
                        isEdge = true;
                    }
                }
                if (particleMatrix.inBounds(y + 1, x))
                {
                    if (particleMatrix.get(y + 1, x).playerCollisionType != collisionType)
                    {
                        isEdge = true;
                    }
                }

                binaryCollisionMatrix.set(y, x, isEdge ? 1 : 0);
            }
        }
    }

    return binaryCollisionMatrix;
}