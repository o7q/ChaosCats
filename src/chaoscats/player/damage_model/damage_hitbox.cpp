#include "damage_model.hpp"

// collision mesh generator/updater
// implements: https://en.wikipedia.org/wiki/Marching_squares
void DamageModel::updateHitbox(int bodyPartX, int bodyPartY, int playerScale)
{
    damageHitbox.clear();

    for (int y = 0; y < sizeY - 1; ++y)
    {
        for (int x = 0; x < sizeX - 1; ++x)
        {
            // [d][c]
            // [a][b]

            int a = binaryHitbox.get(y + 1, x    );
            int b = binaryHitbox.get(y + 1, x + 1);
            int c = binaryHitbox.get(y,     x + 1);
            int d = binaryHitbox.get(y,     x    );

            int lookupIndex = getLookupIndex(a, b, c, d);

            appendLookupSegment(lookupIndex, x, y, bodyPartX, bodyPartY, playerScale);
        }
    }
}

int DamageModel::getLookupIndex(int a, int b, int c, int d)
{
    // 8   4
    //  [-]
    // 1   2
    return a * 1 + b * 2 + c * 4 + d * 8;
}

void DamageModel::appendLookupSegment(int index, int x, int y, int bodyPartX, int bodyPartY, int playerScale)
{
    float half_scale = playerScale / 2.0f;
    Vector<float> l(
        (x + 0  ) * playerScale + half_scale + bodyPartX - (sizeX * playerScale) / 2,
        (y + 0.5) * playerScale + half_scale + bodyPartY - (sizeY * playerScale) / 2);
    Vector<float> b(
        (x + 0.5) * playerScale + half_scale + bodyPartX - (sizeX * playerScale) / 2,
        (y + 1  ) * playerScale + half_scale + bodyPartY - (sizeY * playerScale) / 2);
    Vector<float> r(
        (x + 1  ) * playerScale + half_scale + bodyPartX - (sizeX * playerScale) / 2,
        (y + 0.5) * playerScale + half_scale + bodyPartY - (sizeY * playerScale) / 2);
    Vector<float> t(
        (x + 0.5) * playerScale + half_scale + bodyPartX - (sizeX * playerScale) / 2,
        (y + 0  ) * playerScale + half_scale + bodyPartY - (sizeY * playerScale) / 2);

    switch (index)
    {
    case 0:
        break;
    case 1:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(l, b), x, y});
        break;
    case 2:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(b, r), x, y});
        break;
    case 3:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(l, r), x, y});
        break;
    case 4:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(t, r), x, y});
        break;
    case 5:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(l, t), x, y});
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(b, r), x, y});
        break;
    case 6:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(t, b), x, y});
        break;
    case 7:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(l, t), x, y});
        break;
    case 8:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(l, t), x, y});
        break;
    case 9:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(t, b), x, y});
        break;
    case 10:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(l, b), x, y});
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(t, r), x, y});
        break;
    case 11:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(t, r), x, y});
        break;
    case 12:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(l, r), x, y});
        break;
    case 13:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(b, r), x, y});
        break;
    case 14:
        damageHitbox.push_back(DamageModelEdgeData{Segment<float>(l, b), x, y});
        break;
    case 15:
        break;
    }
}