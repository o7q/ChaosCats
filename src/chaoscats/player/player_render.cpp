#include "player.hpp"

void Player::render(sf::RenderWindow &renderWindow, Camera *camera, const std::string &name)
{
    if (isDead())
    {
        return;
    }

    sf::Vector2i flipVector = facingRight ? sf::Vector2i(1, 1) : sf::Vector2i(-1, 1);

    // draw head
    drawTexture(
        renderWindow,
        gameState.window,
        damageModel.head->getTexture(),
        sf::Vector2f(head_boundingBox.getPosition().x - scale / 2,
                     head_boundingBox.getPosition().y - scale / 2),
        sf::Vector2f(head_boundingBox.size.x,
                     head_boundingBox.size.y),
        camera, 0.0f, flipVector);
    // draw body
    drawTexture(
        renderWindow,
        gameState.window,
        damageModel.body->getTexture(),
        sf::Vector2f(body_boundingBox.getPosition().x - scale / 2,
                     body_boundingBox.getPosition().y - scale / 2),
        sf::Vector2f(body_boundingBox.size.x,
                     body_boundingBox.size.y),
        camera, 0.0f, flipVector);

    // draw legs
    drawTexture(renderWindow,
                gameState.window,
                gameAssets.textures.PLAYER_LEG_DEFAULT,
                sf::Vector2f(leftLeg_boundingBox.getPosition().x - scale,
                             leftLeg_boundingBox.getPosition().y),
                sf::Vector2f(leftLeg_boundingBox.size.x,
                             leftLeg_boundingBox.size.y),
                camera);
    drawTexture(renderWindow,
                gameState.window,
                gameAssets.textures.PLAYER_LEG_DEFAULT,
                sf::Vector2f(rightLeg_boundingBox.getPosition().x + scale,
                             rightLeg_boundingBox.getPosition().y),
                sf::Vector2f(-rightLeg_boundingBox.size.x,
                             rightLeg_boundingBox.size.y),
                camera);
    //

    // draw hands
    std::pair<Vector<float>, Vector<float>> handPlacements = weapon->getHandPlacementPoints(position.x, position.y);

    drawTexture(renderWindow,
                gameState.window,
                gameAssets.textures.PLAYER_HAND_DEFAULT,
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
                gameAssets.textures.PLAYER_HAND_DEFAULT,
                sf::Vector2f(handPlacements.first.x,
                             handPlacements.first.y),
                sf::Vector2f(15, -1),
                camera);

    if (renderDebug)
    {
        // bounding boxes
        drawRect(renderWindow, gameState.window, body_boundingBox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);

        drawRect(renderWindow, gameState.window, head_boundingBox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);

        drawRect(renderWindow, gameState.window, leftLeg_boundingBox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        drawRect(renderWindow, gameState.window, rightLeg_boundingBox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
        //

        // collision box
        drawRect(renderWindow, gameState.window, boundingBox, camera, sf::Color(0, 255, 0, 255), DrawMode::Outline);
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

    drawText(renderWindow, gameState.window, name, gameAssets.fonts.ALAGARD_FONT, sf::Vector2f(collisionSegment.p1.x - head_boundingBox.half_size.x, collisionSegment.p1.y - 30), 10, camera, sf::Color(100, 255, 0, 255));
}