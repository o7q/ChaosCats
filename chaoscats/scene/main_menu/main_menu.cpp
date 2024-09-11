#include "main_menu.hpp"

#include "../../math/matrix/matrix.hpp"
#include "../../math/matrix/matrix_math.hpp"

#include <vector>

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "Main menu init" << std::endl;

        player = new Player(renderWindow, gameAssets.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));
        world = new World(renderWindow, 220, 120, 5);
    }

    if (update())
    {
        Particle drawParticle;
        drawParticle.color = sf::Color(255, 255, 0, 255);
        drawParticle.matterType = MatterType::Liquid;
        drawParticle.physicsType = PhysicsType::Water;
        drawParticle.playerCollisionType = PlayerCollisionType::Solid;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            drawParticle.color = sf::Color(0, 0, 0, 0);
            drawParticle.matterType = MatterType::None;
            drawParticle.physicsType = PhysicsType::None;
            drawParticle.playerCollisionType = PlayerCollisionType::None;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i localMouse = sf::Mouse::getPosition(renderWindow);
            world->drawParticles(localMouse.x / 5, localMouse.y / 5, drawParticle, 8);
        }

        world->update();
        world->render();
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}