#include "button.hpp"

#include "../rendering/draw.hpp"
#include "../math/types/types_common.hpp"

Button::Button(float x, float y, float w, float h, std::string text, sf::Font &font) : font(font)
{
    this->position_x = x;
    this->position_y = y;
    this->size_x = w;
    this->size_y = h;
    this->text = text;
}
Button::~Button()
{
}

void Button::update(GameState gameState)
{
    mouse_hovering = handle_mouse_hover(gameState);
    mouse_down = handle_mouse_down(gameState.input);
    mouse_clicked = handle_mouse_click(gameState.input);
}

bool Button::isMouseHovering()
{
    return mouse_hovering;
}

bool Button::isMouseDown()
{
    return mouse_down;
}

bool Button::hasMouseClicked()
{
    return mouse_clicked;
}

bool Button::handle_mouse_hover(GameState gameState)
{
    sf::Vector2f mousePos = point_screenToWorld(sf::Vector2f(gameState.input.MOUSE_POS.x, gameState.input.MOUSE_POS.y), nullptr, gameState.window);
    return mousePos.x > position_x - size_x / 2.0f &&
           mousePos.x < position_x + size_x / 2.0f &&
           mousePos.y > position_y - size_y / 2.0f &&
           mousePos.y < position_y + size_y / 2.0f;
}

bool Button::handle_mouse_down(GameState::InputState inputState)
{
    return mouse_hovering && inputState.MOUSE_LMB_DOWN;
}

bool Button::handle_mouse_click(GameState::InputState inputState)
{
    return mouse_hovering && inputState.MOUSE_LMB_CLICKED;
}

void Button::render(sf::RenderWindow &renderWindow, GameState gameState)
{
    sf::Color buttonColor = mouse_hovering ? sf::Color(0, 255, 0, 255) : sf::Color(255, 0, 0, 255);
    sf::Color textColor = mouse_hovering ? sf::Color(255, 0, 0, 255) : sf::Color(0, 255, 0, 255);
    drawRect(renderWindow, gameState.window, Rect<float>(position_x, position_y, size_x, size_y), nullptr, buttonColor);
    drawText(renderWindow, gameState.window, text, font, sf::Vector2f(position_x, position_y), 20, nullptr, textColor);
}