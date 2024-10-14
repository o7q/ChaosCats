#include "weapon.hpp"

void Weapon::initHandPlacementPoint(sf::Texture &weaponTexture, sf::Texture &weaponPointsTexture)
{
    sf::Vector2u textureSize = weaponTexture.getSize();

    sf::Vector2f aspect = imageResize(textureSize, sf::Vector2f(textureSize.x * scale, -1));

    sf::Image gunPointsImage = weaponPointsTexture.copyToImage();

    for (unsigned int y = 0; y < gunPointsImage.getSize().y; ++y)
    {
        for (unsigned int x = 0; x < gunPointsImage.getSize().x; ++x)
        {
            sf::Color pixColor = gunPointsImage.getPixel(x, y);
            if (pixColor == sf::Color(255, 0, 0, 255))
            {
                muzzlePosition = Vector<int>(x, y);
            }
            else if (pixColor == sf::Color(0, 255, 0, 255))
            {
                handMountPosition1 = Vector<int>(x, y);
            }
            else if (pixColor == sf::Color(0, 0, 255, 255))
            {
                handMountPosition2 = Vector<int>(x, y);
            }
        }
    }

    weaponSize.x = aspect.x;
    weaponSize.y = aspect.y;
}

std::pair<Vector<float>, Vector<float>> Weapon::getHandPlacementPoints(float x, float y)
{
    Vector<float> handPosition1 = rotatePoint(Vector<float>(
                                                  handMountPosition1.x * scale - weaponSize.x / 2,
                                                  handMountPosition1.y * scale - weaponSize.y / 2),
                                              rotation_radians);
    Vector<float> handPosition2 = rotatePoint(Vector<float>(
                                                  handMountPosition2.x * scale - weaponSize.x / 2,
                                                  handMountPosition2.y * scale - weaponSize.y / 2),
                                              rotation_radians);

    handPosition1.x += x + weaponPositionOffset.x;
    handPosition1.y += y + weaponPositionOffset.y;
    handPosition2.x += x + weaponPositionOffset.x;
    handPosition2.y += y + weaponPositionOffset.y;

    return std::pair<Vector<float>, Vector<float>>(handPosition1, handPosition2);
}