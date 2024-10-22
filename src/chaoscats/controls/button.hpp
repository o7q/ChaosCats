#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "../game_state/game_state.hpp"

class Button
{
public:
    Button(float x, float y, float w, float h, std::string text, sf::Font& font);
    ~Button();

    void update(GameState gameState);

    void render(sf::RenderWindow &renderWindow, GameState gameState);

    bool isMouseHovering();
    bool isMouseDown();
    bool hasMouseClicked();

private:
    float position_x = 0.0f;
    float position_y = 0.0f;
    float size_x = 0.0f;
    float size_y = 0.0f;
    std::string text;
    sf::Font& font;

    bool mouse_hovering = false;
    bool mouse_down = false;
    bool mouse_clicked = false;

    bool handle_mouse_hover(GameState gameState);
    bool handle_mouse_down(GameState::InputState inputState);
    bool handle_mouse_click(GameState::InputState inputState);
};