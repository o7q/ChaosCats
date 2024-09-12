#include "main_menu.hpp"

#include "../../math/matrix/matrix.hpp"

#include <vector>

CurrentScene Scene_MainMenu::run()
{
    if (init())
    {
        std::cout << "Main menu init" << std::endl;

        player = new Player(renderWindow, gameAssets.TEXTURES.TEST_PLAYER_TEXTURE, sf::Vector2f(100, -1));
        world = new World(renderWindow, 6, 6, 5);
    }

    if (update())
    {
        Particle drawParticle;
        drawParticle.color = sf::Color(255, 255, 0, 255);
        drawParticle.matterType = MatterType::Solid;
        drawParticle.physicsType = PhysicsType::Sand;
        drawParticle.playerCollisionType = PlayerCollisionType::Solid;
        world->setParticle(0, 5, drawParticle);
        world->setParticle(1, 5, drawParticle);
        world->setParticle(2, 5, drawParticle);
        world->setParticle(3, 5, drawParticle);
        world->setParticle(4, 5, drawParticle);
        // world->setParticle(5, 5, drawParticle);
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        // {
        //     drawParticle.color = sf::Color(0, 0, 0, 0);
        //     drawParticle.matterType = MatterType::None;
        //     drawParticle.physicsType = PhysicsType::None;
        //     drawParticle.playerCollisionType = PlayerCollisionType::None;
        // }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        // {
        //     drawParticle.color = sf::Color(0, 0, 255, 255);
        //     drawParticle.matterType = MatterType::Liquid;
        //     drawParticle.physicsType = PhysicsType::Water;
        //     drawParticle.playerCollisionType = PlayerCollisionType::Solid;
        // }

        // if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        // {
        //     sf::Vector2i localMouse = sf::Mouse::getPosition(renderWindow);
        //     world->drawParticles(localMouse.x / 5, localMouse.y / 5, drawParticle, 8);
        // }

        world->update();
        world->render();
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}