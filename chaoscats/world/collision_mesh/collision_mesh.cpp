#include "collision_mesh.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

std::vector<Segment<int>> generateCollisionMesh(Mat2D<int> &hollowBinaryMatrix, int scale)
{
    int rowSize = hollowBinaryMatrix.getRowSize();
    int colSize = hollowBinaryMatrix.getColSize();

    std::vector<Segment<int>> segments;

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

        Point2D<int> moveVector;
        Point2D<int> lastMoveVector;

        Point2D<int> checkpoint = headPoint;

        bool finishedLine = false;

        auto pushSegment = [&]()
        {
            if (moveVector.x != lastMoveVector.x || moveVector.y != lastMoveVector.y)
            {
                segments.push_back(Segment<int>(
                    Point2D<int>(checkpoint.x * scale + scale / 2, checkpoint.y * scale + scale / 2),
                    Point2D<int>(headPoint.x * scale + scale / 2, headPoint.y * scale + scale / 2)));
                checkpoint = headPoint;
            }
        };

        while (!finishedLine)
        {
            // 8  7  6
            // 1 [-] 5
            // 2  3  4

            lastMoveVector = moveVector;

            if (hollowBinaryMatrix.inBounds(headPoint.y, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y, headPoint.x - 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    moveVector.x = -1;
                    moveVector.y = 0;

                    pushSegment();

                    headPoint.x += -1;
                    headPoint.y += 0;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x - 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    moveVector.x = -1;
                    moveVector.y = 1;

                    pushSegment();

                    headPoint.x += -1;
                    headPoint.y += 1;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    moveVector.x = 0;
                    moveVector.y = 1;

                    pushSegment();

                    headPoint.x += 0;
                    headPoint.y += 1;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y + 1, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y + 1, headPoint.x + 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    moveVector.x = 1;
                    moveVector.y = 1;

                    pushSegment();

                    headPoint.x += 1;
                    headPoint.y += 1;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y, headPoint.x + 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    moveVector.x = 1;
                    moveVector.y = 0;

                    pushSegment();

                    headPoint.x += 1;
                    headPoint.y += 0;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y - 1, headPoint.x + 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y - 1, headPoint.x + 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    moveVector.x = 1;
                    moveVector.y = -1;

                    pushSegment();

                    headPoint.x += 1;
                    headPoint.y += -1;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y - 1, headPoint.x))
            {
                if (hollowBinaryMatrix.get(headPoint.y - 1, headPoint.x) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    moveVector.x = 0;
                    moveVector.y = -1;

                    pushSegment();

                    headPoint.x += 0;
                    headPoint.y += -1;

                    continue;
                }
            }
            if (hollowBinaryMatrix.inBounds(headPoint.y - 1, headPoint.x - 1))
            {
                if (hollowBinaryMatrix.get(headPoint.y - 1, headPoint.x - 1) == 1)
                {
                    hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

                    moveVector.x = -1;
                    moveVector.y = -1;

                    pushSegment();

                    headPoint.x += -1;
                    headPoint.y += -1;

                    continue;
                }
            }

            pushSegment();
            hollowBinaryMatrix.set(headPoint.y, headPoint.x, 0);

            finishedLine = true;
        }
    }

    // remove garbage segments (segments that point1 == point2)
    segments.erase(std::remove_if(segments.begin(),
                                  segments.end(),
                                  [](Segment<int> segment)
                                  {
                                      return segment.p1.x == segment.p2.x && segment.p1.y == segment.p2.y;
                                  }),
                   segments.end());

    return segments;
}