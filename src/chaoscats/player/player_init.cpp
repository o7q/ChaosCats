#include "player.hpp"

void Player::initPlayer()
{
    sf::Vector2u headTextureSize = gameAssets.textures.PLAYER_HEAD_TEXTURE.getSize();
    sf::Vector2f sf_head_size = imageResize(headTextureSize, sf::Vector2f(headTextureSize.x * scale, -1));
    Vector<float> head_size = Vector<float>(sf_head_size.x, sf_head_size.y);
    head_hitbox = Rect<float>(0, 0, head_size.x, head_size.y);

    sf::Vector2u bodyTextureSize = gameAssets.textures.PLAYER_BODY_TEXTURE.getSize();
    sf::Vector2f sf_body_size = imageResize(bodyTextureSize, sf::Vector2f(bodyTextureSize.x * scale, -1));
    Vector<float> body_size = Vector<float>(sf_body_size.x, sf_body_size.y);
    body_hitbox = Rect<float>(0, 0, body_size.x, body_size.y);

    sf::Vector2u legTextureSize = gameAssets.textures.PLAYER_LEG_TEXTURE.getSize();
    sf::Vector2f sf_leg_size = imageResize(legTextureSize, sf::Vector2f(legTextureSize.x * scale, -1));
    Vector<float> leftLeg_size = Vector<float>(sf_leg_size.x, sf_leg_size.y);
    leftLeg_hitbox = Rect<float>(0, 0, leftLeg_size.x, leftLeg_size.y);
    Vector<float> rightLeg_size = Vector<float>(sf_leg_size.x, sf_leg_size.y);
    rightLeg_hitbox = Rect<float>(0, 0, rightLeg_size.x, rightLeg_size.y);

    player_collisionBox = Rect<float>(0, 0, body_hitbox.size.x, body_hitbox.size.y + head_hitbox.size.y);

    std::vector<sf::Texture *> headGoreLayers;
    headGoreLayers.push_back(&gameAssets.textures.PLAYER_HEAD_TEXTURE);
    headGoreLayers.push_back(&gameAssets.textures.PLAYER_HEAD_GORE1_TEXTURE);
    headGoreLayers.push_back(&gameAssets.textures.PLAYER_HEAD_GORE2_TEXTURE);
    headGoreLayers.push_back(&gameAssets.textures.PLAYER_HEAD_GORE3_TEXTURE);
    damageModel.head = new DamageModel(headTextureSize.x, headTextureSize.y, headGoreLayers);

    std::vector<sf::Texture *> bodyGoreLayers;
    bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_TEXTURE);
    bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE1_TEXTURE);
    bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE2_TEXTURE);
    bodyGoreLayers.push_back(&gameAssets.textures.PLAYER_BODY_GORE3_TEXTURE);
    damageModel.body = new DamageModel(bodyTextureSize.x, bodyTextureSize.y, bodyGoreLayers);

    weapon = new Shotgun(world, gameState, gameAssets);
}