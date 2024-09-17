#include <iostream>
#include <vector>

#include "collision_mesh.hpp"
#include "../particle.hpp"

// collision mesh generator/updater
// implements: https://en.wikipedia.org/wiki/Marching_squares
void updateCollisionMesh(std::vector<Segment<float>> &collisionMesh, Matrix<Particle> &particleWorld, PlayerCollisionType collisionType, int scale)
{
    int rowSize = particleWorld.getRowSize();
    int colSize = particleWorld.getColSize();

    collisionMesh.clear();

    for (int y = 0; y < rowSize - 1; ++y)
    {
        for (int x = 0; x < colSize - 1; ++x)
        {
            // [d][c]
            // [a][b]

            int a = particleWorld.get(y + 1, x    ).playerCollisionType == collisionType ? 1 : 0;
            int b = particleWorld.get(y + 1, x + 1).playerCollisionType == collisionType ? 1 : 0;
            int c = particleWorld.get(y    , x + 1).playerCollisionType == collisionType ? 1 : 0;
            int d = particleWorld.get(y    , x    ).playerCollisionType == collisionType ? 1 : 0;

            int lookupIndex = getLookupIndex(a, b, c, d);

            appendLookupSegment(collisionMesh, lookupIndex, x, y, scale);
        }
    }
}

float slope(int x, int y)
{
    return y / static_cast<float>(x);
}

int getLookupIndex(int a, int b, int c, int d)
{
    // 8   4
    //  [-]
    // 1   2
    return a * 1 + b * 2 + c * 4 + d * 8;
}

void appendLookupSegment(std::vector<Segment<float>> &segments, int index, int x, int y, int scale)
{
    int half_scale = scale / 2;
    Point<float> left(
        (x + 0  ) * scale + half_scale,
        (y + 0.5) * scale + half_scale);
    Point<float> bottom(
        (x + 0.5) * scale + half_scale,
        (y + 1  ) * scale + half_scale);
    Point<float> right(
        (x + 1  ) * scale + half_scale,
        (y + 0.5) * scale + half_scale);
    Point<float> top(
        (x + 0.5) * scale + half_scale,
        (y + 0  ) * scale + half_scale);

    switch (index)
    {
    case 0:
        break;
    case 1:
        segments.push_back(Segment<float>(left, bottom));
        break;
    case 2:
        segments.push_back(Segment<float>(bottom, right));
        break;
    case 3:
        segments.push_back(Segment<float>(left, right));
        break;
    case 4:
        segments.push_back(Segment<float>(top, right));
        break;
    case 5:
        segments.push_back(Segment<float>(left, top));
        segments.push_back(Segment<float>(bottom, right));
        break;
    case 6:
        segments.push_back(Segment<float>(top, bottom));
        break;
    case 7:
        segments.push_back(Segment<float>(left, top));
        break;
    case 8:
        segments.push_back(Segment<float>(left, top));
        break;
    case 9:
        segments.push_back(Segment<float>(top, bottom));
        break;
    case 10:
        segments.push_back(Segment<float>(left, bottom));
        segments.push_back(Segment<float>(top, right));
        break;
    case 11:
        segments.push_back(Segment<float>(top, right));
        break;
    case 12:
        segments.push_back(Segment<float>(left, right));
        break;
    case 13:
        segments.push_back(Segment<float>(bottom, right));
        break;
    case 14:
        segments.push_back(Segment<float>(left, bottom));
        break;
    case 15:
        break;
    }
}