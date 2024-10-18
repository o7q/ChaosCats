#pragma once

#include <iostream>
#include <cmath>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../../math/types/types_common.hpp"
#include "../../math/matrix.hpp"
#include "../../math/random.hpp"

struct DamageModelEdgeData
{
    Segment<float> segment;
    int localX = 0;
    int localY = 0;
};

class DamageModel
{
public:
    DamageModel(int sizeX,
                int sizeY,
                const std::vector<const sf::Texture *> &goreLayerTextures) : damageIndexes(sizeX, sizeY),
                                                                             binaryHitbox(sizeX, sizeY)
    {
        for (const sf::Texture *texture : goreLayerTextures)
        {
            sf::Image temp = texture->copyToImage();
            goreLayers.push_back(temp);
        }

        this->sizeX = sizeX;
        this->sizeY = sizeY;

        for (int y = 0; y < sizeY; ++y)
        {
            for (int x = 0; x < sizeX; ++x)
            {
                // init default values
                damageIndexes.set(y, x, 0);

                int alpha = goreLayers[0].getPixel(x, y).a == 0 ? 0 : 1;
                binaryHitbox.set(y, x, alpha);
            }
        }

        modelPixels = new sf::Uint8[sizeX * sizeY * 4];
        modelTexture.loadFromImage(goreLayers[0]);
    }
    ~DamageModel()
    {
        if (modelPixels != nullptr)
        {
            delete[] modelPixels;
            modelPixels = nullptr;
        }
    }

    void damage(int locationX, int locationY, int radius)
    {
        for (int i = radius; i > 0; --i)
        {
            damageCircle(locationX, locationY, i);
        }

        for (int y = 0; y < sizeY; ++y)
        {
            for (int x = 0; x < sizeX; ++x)
            {
                int goreIndex = damageIndexes.get(y, x);
                sf::Color gorePixel = goreLayers[goreIndex].getPixel(x, y);

                int index = (y * sizeX + x) * 4;
                modelPixels[index + 0] = gorePixel.r;
                modelPixels[index + 1] = gorePixel.g;
                modelPixels[index + 2] = gorePixel.b;
                modelPixels[index + 3] = gorePixel.a;
            }
        }

        modelTexture.update(modelPixels);
    }

    void update(int bodyPartX, int bodyPartY, int playerScale)
    {
        updateHitbox(bodyPartX, bodyPartY, playerScale);
    }

    std::vector<DamageModelEdgeData> &getDamageHitbox()
    {
        return damageHitbox;
    }

    sf::Texture &getTexture()
    {
        return modelTexture;
    }

private:
    int sizeX;
    int sizeY;

    // indicies for damage levels, higher levels = deeper gore texture
    Matrix<int> damageIndexes;
    // hitbox positions for marching squares
    Matrix<int> binaryHitbox;

    // the hitbox itself
    std::vector<DamageModelEdgeData> damageHitbox;
    // each gore layer
    std::vector<sf::Image> goreLayers;

    sf::Uint8 *modelPixels = nullptr;
    sf::Texture modelTexture;

    void updateHitbox(int bodyPartX, int bodyPartY, int playerScale);
    int getLookupIndex(int a, int b, int c, int d);
    void appendLookupSegment(int index, int x, int y, int bodyPartX, int bodyPartY, int playerScale);

    void damageCircle(int locationX, int locationY, int radius)
    {
        for (int y = locationY - radius; y <= locationY + radius; ++y)
        {
            for (int x = locationX - radius; x <= locationX + radius; ++x)
            {
                if ((x - locationX) * (x - locationX) + (y - locationY) * (y - locationY) <= radius * radius)
                {
                    int x_clamped = std::clamp(x, 0, sizeX - 1);
                    int y_clamped = std::clamp(y, 0, sizeY - 1);

                    int upper = goreLayers.size() - 1;
                    int damageChance = Random::genInt(0, 3);
                    int damageValue = std::clamp(damageIndexes.get(y_clamped, x_clamped) + (damageChance == 0 ? 1 : 0), 0, upper);

                    damageIndexes.set(y_clamped, x_clamped, damageValue);

                    if (damageValue == upper)
                    {
                        binaryHitbox.set(y_clamped, x_clamped, 0);
                    }
                }
            }
        }
    }
};

struct PlayerDamageModel
{
    DamageModel *head = nullptr;
    DamageModel *body = nullptr;

    ~PlayerDamageModel()
    {
        if (head != nullptr)
        {
            delete head;
            head = nullptr;
        }
        if (body != nullptr)
        {
            delete body;
            body = nullptr;
        }
    }
};