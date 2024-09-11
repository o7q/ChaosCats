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
        world = new World(renderWindow, 128, 80, 5);
    }

    if (update())
    {
        Particle drawParticle;
        drawParticle.color = sf::Color(255, 255, 0, 255);
        drawParticle.matterType = MatterType::Solid;
        drawParticle.physicsType = PhysicsType::Sand;
        drawParticle.playerCollisionType = PlayerCollisionType::Solid;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i localMouse = sf::Mouse::getPosition(renderWindow);
            world->drawParticles(localMouse.x / 5, localMouse.y / 5, drawParticle, 2);
        }

        world->update();
        world->render();
    }

    rot++;

    return CurrentScene::SCENE_MAIN_MENU;
}