#include "collision_mesh.hpp"
#include "../geometry/point.hpp"

#include <iostream>

std::vector<Point<int>> pointsFromHollowBinaryMatrix(Matrix2D<int> &hollowBinaryMatrix)
{
    int rowSize = hollowBinaryMatrix.getRowSize();
    int colSize = hollowBinaryMatrix.getColSize();

    std::vector<Point<int>> points;

    bool complete = false;

    while (!complete)
    {
        bool break2 = false;

        bool valuesExist = false;

        Point<int> headPoint;
        for (int y = 0; y < rowSize; ++y)
        {
            for (int x = 0; x < colSize; ++x)
            {
                int value = hollowBinaryMatrix.get(y, x);
                if (value == 1)
                {
                    headPoint = Point<int>(x, y);
                    break2 = true;
                    valuesExist = true;
                    break;
                }
            }

            if (break2)
            {
                break;
            }
        }

        if (!valuesExist)
        {
            complete = true;
            break;
        }

        Point<int> moveVector;
        Point<int> lastMoveVector;
        bool finishedLine = false;

        while (!finishedLine)
        {
            //
            // 1 [-] 5
            // 2  3  4

            lastMoveVector = moveVector;

            if (hollowBinaryMatrix.inBounds(headPoint.y, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y, headPoint.x - 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x - 1, 0);
                    headPoint.x -= 1;

                    moveVector.x = -1;
                    moveVector.y = 0;

                    if (moveVector.x != lastMoveVector.x || moveVector.y != lastMoveVector.y)
                    {
                        points.push_back(Point<int>(headPoint.x, headPoint.y));
                    }

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x - 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y + 1, headPoint.x - 1, 0);
                    headPoint.x -= 1;
                    headPoint.y += 1;

                    moveVector.x = -1;
                    moveVector.y = 1;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y + 1, headPoint.x, 0);
                    headPoint.y += 1;

                    moveVector.x = 0;
                    moveVector.y = 1;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x + 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y + 1, headPoint.x + 1, 0);
                    headPoint.x += 1;
                    headPoint.y += 1;

                    moveVector.x = 1;
                    moveVector.y = 1;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y, headPoint.x + 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x + 1, 0);
                    headPoint.x += 1;

                    moveVector.x = 1;
                    moveVector.y = 0;

                    continue;
                }
            }

            finishedLine = true;
        }
    }

    return points;
}