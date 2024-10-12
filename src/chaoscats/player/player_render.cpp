#include "player.hpp"

void Player::render(sf::RenderWindow &renderWindow, Camera *camera, const std::string &name)
{
    // draw head
    drawTexture(
        renderWindow,
        gameInfo.window,
        damageModel.head->getTexture(),
        sf::Vector2f(head_hitbox.getPosition().x - scale / 2,
                     head_hitbox.getPosition().y - scale / 2),
        sf::Vector2f(head_hitbox.size.x,
                     head_hitbox.size.y),
        camera);
    // draw body
    drawTexture(
        renderWindow,
        gameInfo.window,
        damageModel.body->getTexture(),
        sf::Vector2f(body_hitbox.getPosition().x - scale / 2,
                     body_hitbox.getPosition().y - scale / 2),
        sf::Vector2f(body_hitbox.size.x,
                     body_hitbox.size.y),
        camera);

    // draw legs
    drawTexture(renderWindow,
                gameInfo.window,
                gameAssets.textures.PLAYER_LEG_TEXTURE,
                sf::Vector2f(leftLeg_hitbox.getPosition().x - scale,
                             leftLeg_hitbox.getPosition().y),
                sf::Vector2f(leftLeg_hitbox.size.x,
                             leftLeg_hitbox.size.y),
                camera);
    drawTexture(renderWindow,
                gameInfo.window,
                gameAssets.textures.PLAYER_LEG_TEXTURE,
                sf::Vector2f(rightLeg_hitbox.getPosition().x + scale,
                             rightLeg_hitbox.getPosition().y),
                sf::Vector2f(-rightLeg_hitbox.size.x,
                             rightLeg_hitbox.size.y),
                camera);
    //

    // // draw hands
    std::pair<Vector<float>, Vector<float>> handPlacements = weapon->getHandPlacementPoints(position.x, position.y);

    drawTexture(renderWindow,
                gameInfo.window,
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
                gameInfo.window,
                gameAssets.textures.PLAYER_HAND_TEXTURE,
                sf::Vector2f(handPlacements.first.x,
                             handPlacements.first.y),
                sf::Vector2f(15, -1),
                camera);

    if (renderHitboxes)
    {
        // bounding boxes
        drawRect(renderWindow, gameInfo.window, body_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);

        drawRect(renderWindow, gameInfo.window, head_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);

        drawRect(renderWindow, gameInfo.window, leftLeg_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        drawRect(renderWindow, gameInfo.window, rightLeg_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        //

        // collision box
        drawRect(renderWindow, gameInfo.window, player_collisionBox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        drawPoint(renderWindow, gameInfo.window, bottomCollisionPoint, camera, sf::Color(255, 0, 0, 255));
        drawPoint(renderWindow, gameInfo.window, topCollisionPoint, camera, sf::Color(255, 0, 0, 255));
        //

        drawPoint(renderWindow, gameInfo.window, Vector<float>(player_collisionBox.getPosition().x, player_collisionBox.getPosition().y), camera, sf::Color(0, 255, 0, 255));

        // dynamic hit box
        for (DamageModelEdgeData dmed : damageModel.head->getDamageHitbox())
        {
            drawSegment(renderWindow, gameInfo.window, dmed.segment, camera, sf::Color(0, 255, 255, 255));
        }

        for (DamageModelEdgeData dmed : damageModel.body->getDamageHitbox())
        {
            drawSegment(renderWindow, gameInfo.window, dmed.segment, camera, sf::Color(0, 255, 255, 255));
        }
        //
    }

    drawText(renderWindow, gameInfo.window, name, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(topCollisionPoint.x - head_hitbox.half_size.x, topCollisionPoint.y - 30), 10, camera, sf::Color(100, 255, 0, 255));
}