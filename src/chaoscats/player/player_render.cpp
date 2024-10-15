#include "player.hpp"

void Player::render(sf::RenderWindow &renderWindow, Camera *camera, const std::string &name)
{
    if (isDead())
    {
        return;
    }

    // draw head
    drawTexture(
        renderWindow,
        gameState.window,
        damageModel.head->getTexture(),
        sf::Vector2f(head_hitbox.getPosition().x - scale / 2,
                     head_hitbox.getPosition().y - scale / 2),
        sf::Vector2f(head_hitbox.size.x,
                     head_hitbox.size.y),
        camera);
    // draw body
    drawTexture(
        renderWindow,
        gameState.window,
        damageModel.body->getTexture(),
        sf::Vector2f(body_hitbox.getPosition().x - scale / 2,
                     body_hitbox.getPosition().y - scale / 2),
        sf::Vector2f(body_hitbox.size.x,
                     body_hitbox.size.y),
        camera);

    // draw legs
    drawTexture(renderWindow,
                gameState.window,
                gameAssets.textures.PLAYER_LEG_TEXTURE,
                sf::Vector2f(leftLeg_hitbox.getPosition().x - scale,
                             leftLeg_hitbox.getPosition().y),
                sf::Vector2f(leftLeg_hitbox.size.x,
                             leftLeg_hitbox.size.y),
                camera);
    drawTexture(renderWindow,
                gameState.window,
                gameAssets.textures.PLAYER_LEG_TEXTURE,
                sf::Vector2f(rightLeg_hitbox.getPosition().x + scale,
                             rightLeg_hitbox.getPosition().y),
                sf::Vector2f(-rightLeg_hitbox.size.x,
                             rightLeg_hitbox.size.y),
                camera);
    //

    // draw hands
    std::pair<Vector<float>, Vector<float>> handPlacements = weapon->getHandPlacementPoints(position.x, position.y);

    drawTexture(renderWindow,
                gameState.window,
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
                gameState.window,
                gameAssets.textures.PLAYER_HAND_TEXTURE,
                sf::Vector2f(handPlacements.first.x,
                             handPlacements.first.y),
                sf::Vector2f(15, -1),
                camera);

    if (renderHitboxes)
    {
        // bounding boxes
        drawRect(renderWindow, gameState.window, body_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);

        drawRect(renderWindow, gameState.window, head_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);

        drawRect(renderWindow, gameState.window, leftLeg_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        drawRect(renderWindow, gameState.window, rightLeg_hitbox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        //

        // collision box
        drawRect(renderWindow, gameState.window, player_collisionBox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        drawPoint(renderWindow, gameState.window, collisionSegment.p1, camera, sf::Color(255, 0, 0, 255));
        drawPoint(renderWindow, gameState.window, collisionSegment.p2, camera, sf::Color(255, 0, 0, 255));
        drawSegment(renderWindow, gameState.window, collisionSegment, camera, sf::Color(255, 0, 0, 255));
        //

        // dynamic hit box
        for (DamageModelEdgeData dmed : damageModel.head->getDamageHitbox())
        {
            drawSegment(renderWindow, gameState.window, dmed.segment, camera, sf::Color(0, 255, 255, 255));
        }

        for (DamageModelEdgeData dmed : damageModel.body->getDamageHitbox())
        {
            drawSegment(renderWindow, gameState.window, dmed.segment, camera, sf::Color(0, 255, 255, 255));
        }
        //
    }

    drawText(renderWindow, gameState.window, name, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(collisionSegment.p1.x - head_hitbox.half_size.x, collisionSegment.p1.y - 30), 10, camera, sf::Color(100, 255, 0, 255));
}