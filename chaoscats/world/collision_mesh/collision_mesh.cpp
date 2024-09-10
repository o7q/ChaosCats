#include "collision_mesh.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

std::vector<Point2D<int>> generateCollisionMesh(Mat2D<int> &hollowBinaryMatrix, int scale)
{
    int rowSize = hollowBinaryMatrix.getRowSize();
    int colSize = hollowBinaryMatrix.getColSize();

    std::vector<Point2D<int>> points;

    bool complete = false;

    while (!complete)
    {
        bool break2 = false;

        bool valuesExist = false;

        Point2D<int> headPoint;
        for (int y = 0; y < rowSize; ++y)
        {
            for (int x = 0; x < colSize; ++x)
            {
                int value = hollowBinaryMatrix.get(y, x);
                if (value == 1)
                {
                    headPoint = Point2D<int>(x, y);
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

        auto countNeighbors = [&]()
        {
            int neighborCount = 0;
            if (hollowBinaryMatrix.inBounds(headPoint.y, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y, headPoint.x - 1) == 1)
                {
                    ++neighborCount;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x - 1) == 1)
                {
                    ++neighborCount;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x) == 1)
                {
                    ++neighborCount;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x + 1) == 1)
                {
                    ++neighborCount;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y, headPoint.x + 1) == 1)
                {
                    ++neighborCount;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y - 1, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y - 1, headPoint.x + 1) == 1)
                {
                    ++neighborCount;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y - 1, headPoint.x))
            {
                if (hollowBinaryMatrix.get(headPoint.y - 1, headPoint.x) == 1)
                {
                    ++neighborCount;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y - 1, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y - 1, headPoint.x - 1) == 1)
                {
                    ++neighborCount;
                }
            }

            return neighborCount;
        };

        Point2D<int> moveVector(0, 0);
        Point2D<int> prevMoveVector(0, 0);
        bool finishedPath = false;

        Point2D<int> startPoint(-1, -1);

        while (!finishedPath)
        {
            //
            // 1 [-] 5
            // 2  3  4

            bool thingsHappened = false;

            if (hollowBinaryMatrix.inBounds(headPoint.y, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y, headPoint.x - 1) == 1)
                {
                    bool initial = false;
                    if (startPoint.x == -1 && startPoint.y == -1)
                    {
                        startPoint = headPoint;
                        points.push_back(startPoint);

                        if (countNeighbors() <= 1)
                        {
                            hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);
                        }

                        initial = true;
                    }

                    headPoint.x += -1;
                    headPoint.y += 0;

                    moveVector.x = -1;
                    moveVector.y = 0;

                    if (!initial && (moveVector.x != prevMoveVector.x || moveVector.y != prevMoveVector.y))
                    {
                        points.push_back(headPoint);
                    }

                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    thingsHappened = true;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x - 1) == 1)
                {
                    bool initial = false;
                    if (startPoint.x == -1 && startPoint.y == -1)
                    {
                        startPoint = headPoint;
                        points.push_back(startPoint);

                        if (countNeighbors() <= 1)
                        {
                            hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);
                        }

                        initial = true;
                    }

                    headPoint.x += -1;
                    headPoint.y += 1;

                    moveVector.x = -1;
                    moveVector.y = 1;

                    if (!initial && (moveVector.x != prevMoveVector.x || moveVector.y != prevMoveVector.y))
                    {
                        points.push_back(headPoint);
                    }

                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    thingsHappened = true;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x) == 1)
                {
                    bool initial = false;
                    if (startPoint.x == -1 && startPoint.y == -1)
                    {
                        startPoint = headPoint;
                        points.push_back(startPoint);

                        if (countNeighbors() <= 1)
                        {
                            hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);
                        }

                        initial = true;
                    }

                    headPoint.x += 0;
                    headPoint.y += 1;

                    moveVector.x = 0;
                    moveVector.y = 1;

                    if (!initial && (moveVector.x != prevMoveVector.x || moveVector.y != prevMoveVector.y))
                    {
                        points.push_back(headPoint);
                    }

                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    thingsHappened = true;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x + 1) == 1)
                {
                    bool initial = false;
                    if (startPoint.x == -1 && startPoint.y == -1)
                    {
                        startPoint = headPoint;
                        points.push_back(startPoint);

                        if (countNeighbors() <= 1)
                        {
                            hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);
                        }

                        initial = true;
                    }

                    headPoint.x += 1;
                    headPoint.y += 1;

                    moveVector.x = 1;
                    moveVector.y = 1;

                    if (!initial && (moveVector.x != prevMoveVector.x || moveVector.y != prevMoveVector.y))
                    {
                        points.push_back(headPoint);
                    }

                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    thingsHappened = true;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y, headPoint.x + 1) == 1)
                {
                    bool initial = false;
                    if (startPoint.x == -1 && startPoint.y == -1)
                    {
                        startPoint = headPoint;
                        points.push_back(startPoint);

                        if (countNeighbors() <= 1)
                        {
                            hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);
                        }

                        initial = true;
                    }

                    headPoint.x += 1;
                    headPoint.y += 0;

                    moveVector.x = 1;
                    moveVector.y = 0;

                    if (!initial && (moveVector.x != prevMoveVector.x || moveVector.y != prevMoveVector.y))
                    {
                        points.push_back(headPoint);
                    }

                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    thingsHappened = true;
                }
            }

            prevMoveVector = moveVector;

            if (!thingsHappened)
            {
                points.push_back(headPoint);
                finishedPath = true;
            }
        }
    }

    // remove garbage segments (segments that point1 == point2)
    // segments.erase(std::remove_if(segments.begin(),
    //                               segments.end(),
    //                               [](Segment<int> segment)
    //                               {
    //                                   return segment.p1.x == segment.p2.x && segment.p1.y == segment.p2.y;
    //                               }),
    //                segments.end());

    return points;
}