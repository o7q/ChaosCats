#include "player.hpp"

void Player::render(sf::RenderWindow &renderWindow)
{
    // draw body
    drawTexture(renderWindow, bodyDamageModel->getTexture(), sf::Vector2f(body_hitbox.getPosition().x - scale / 2, body_hitbox.getPosition().y - scale / 2), sf::Vector2f(body_size.x, body_size.y));
    if (renderHitboxes)
    {
        drawRect(renderWindow, body_hitbox, sf::Color(0, 255, 0, 255), DrawMode::Outline);
    }
    //

    // draw head
    drawTexture(renderWindow, headDamageModel->getTexture(), sf::Vector2f(head_hitbox.getPosition().x - scale / 2, head_hitbox.getPosition().y - scale / 2), sf::Vector2f(head_size.x, head_size.y));
    if (renderHitboxes)
    {
        drawRect(renderWindow, head_hitbox, sf::Color(0, 255, 0, 255), DrawMode::Outline);
    }
    //

    // draw legs
    drawTexture(renderWindow, gameAssets.textures.PLAYER_LEG_TEXTURE, sf::Vector2f(leftLeg_hitbox.getPosition().x - scale, leftLeg_hitbox.getPosition().y), sf::Vector2f(leftLeg_size.x, leftLeg_size.y));
    drawTexture(renderWindow, gameAssets.textures.PLAYER_LEG_TEXTURE, sf::Vector2f(rightLeg_hitbox.getPosition().x + scale, rightLeg_hitbox.getPosition().y), sf::Vector2f(-rightLeg_size.x, rightLeg_size.y));
    if (renderHitboxes)
    {
        drawRect(renderWindow, leftLeg_hitbox, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        drawRect(renderWindow, rightLeg_hitbox, sf::Color(0, 255, 0, 255), DrawMode::Outline);
    }
    //

    // draw collisionBox
    if (renderHitboxes)
    {
        drawRect(renderWindow, player_collisionBox, sf::Color(0, 0, 255, 255), DrawMode::Outline);
        drawPoint(renderWindow, bottomCollisionPoint, sf::Color(255, 0, 0, 255));
        drawPoint(renderWindow, topCollisionPoint, sf::Color(255, 0, 0, 255));
    }
    //

    // draw hands
    std::pair<Vector<float>, Vector<float>> handPlacements = weapon->getHandPlacementPoints(position.x, position.y);

    drawTexture(renderWindow, gameAssets.textures.PLAYER_HAND_TEXTURE, sf::Vector2f(handPlacements.second.x, handPlacements.second.y), sf::Vector2f(20, -1));
    weapon->render(renderWindow, position.x, position.y, aimPosition.x, aimPosition.y);
    drawTexture(renderWindow, gameAssets.textures.PLAYER_HAND_TEXTURE, sf::Vector2f(handPlacements.first.x, handPlacements.first.y), sf::Vector2f(20, -1));
    //

    for (DamageModelEdge dme : bodyDamageModel->getDamageHitbox())
    {
        drawSegment(renderWindow, dme.segment, sf::Color(255, 255, 0, 255));
    }
}