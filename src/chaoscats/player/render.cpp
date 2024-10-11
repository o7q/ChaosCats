#include "player.hpp"

void Player::render(sf::RenderWindow &renderWindow, Camera *camera)
{

    // draw head
    drawTexture(
        renderWindow,
        damageModel.head->getTexture(),
        sf::Vector2f(head_hitbox.getPosition().x - scale / 2,
                     head_hitbox.getPosition().y - scale / 2),
        sf::Vector2f(head_hitbox.size.x,
                     head_hitbox.size.y),
        camera);
    // draw body
    drawTexture(
        renderWindow, damageModel.body->getTexture(),
        sf::Vector2f(body_hitbox.getPosition().x - scale / 2,
                     body_hitbox.getPosition().y - scale / 2),
        sf::Vector2f(body_hitbox.size.x,
                     body_hitbox.size.y),
        camera);

    // // draw legs
    drawTexture(renderWindow,
                gameAssets.textures.PLAYER_LEG_TEXTURE,
                sf::Vector2f(leftLeg_hitbox.getPosition().x - scale,
                             leftLeg_hitbox.getPosition().y),
                sf::Vector2f(leftLeg_hitbox.size.x,
                             leftLeg_hitbox.size.y),
                camera);
    drawTexture(renderWindow,
                gameAssets.textures.PLAYER_LEG_TEXTURE,
                sf::Vector2f(rightLeg_hitbox.getPosition().x + scale,
                             rightLeg_hitbox.getPosition().y),
                sf::Vector2f(-rightLeg_hitbox.size.x,
                             rightLeg_hitbox.size.y),
                camera);

    // // draw hands
    std::pair<Vector<float>, Vector<float>> handPlacements = weapon->getHandPlacementPoints(position.x, position.y);

    drawTexture(renderWindow,
                gameAssets.textures.PLAYER_HAND_TEXTURE,
                sf::Vector2f(handPlacements.second.x,
                             handPlacements.second.y),
                sf::Vector2f(15, -1),
                camera);
    weapon->render(renderWindow,
                   camera,
                   position.x,
                   position.y,
                   aimPosition.x,
                   aimPosition.y);
    drawTexture(renderWindow,
                gameAssets.textures.PLAYER_HAND_TEXTURE,
                sf::Vector2f(handPlacements.first.x,
                             handPlacements.first.y),
                sf::Vector2f(15, -1),
                camera);

    if (renderHitboxes)
    {
        // bounding boxes
        drawRect(renderWindow, body_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);

        // drawRect(renderWindow, head_hitbox, sf::Color(0, 255, 0, 255), DrawMode::Outline);

        // drawRect(renderWindow, leftLeg_hitbox, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        // drawRect(renderWindow, rightLeg_hitbox, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        // //

        // // collision box
        // drawRect(renderWindow, player_collisionBox, sf::Color(0, 0, 255, 255), DrawMode::Outline);
        // drawPoint(renderWindow, bottomCollisionPoint, sf::Color(255, 0, 0, 255));
        // drawPoint(renderWindow, topCollisionPoint, sf::Color(255, 0, 0, 255));
        // //

        // // dynamic hit box
        // for (DamageModelEdgeData dmed : damageModel.head->getDamageHitbox())
        // {
        //     drawSegment(renderWindow, dmed.segment, sf::Color(0, 255, 255, 255));
        // }

        // for (DamageModelEdgeData dmed : damageModel.body->getDamageHitbox())
        // {
        //     drawSegment(renderWindow, dmed.segment, sf::Color(0, 255, 255, 255));
        // }
        // //
    }
}