/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIButton.cpp
*/

#include "UIButton.hpp"

raytracer::ui::Button::Button(const Vec2 &position, const Vec2 &size, const std::string &label, sf::Font &font,
    const uint fontSize)
{
    _shape.setPosition(position);
    _shape.setSize(size);
    _shape.setFillColor(sf::Color::White);

    _text.setFont(font);
    _text.setString(label);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(sf::Color::Black);
    _text.setPosition(position + Vec2(10, 10));
}

void raytracer::ui::Button::setOnClick(std::function<void()> callback)
{
    _onClick = std::move(callback);
}

void raytracer::ui::Button::render(sf::RenderWindow &window) noexcept
{
    window.draw(_shape);
    window.draw(_text);
}

void raytracer::ui::Button::update(float __attribute__((unused)) dt) noexcept
{
    /* */
}

void raytracer::ui::Button::onEvent(const sf::Event &event, const sf::RenderWindow &window) noexcept
{
    if (event.type == sf::Event::MouseMoved) {

        const sf::Vector2i mouse_position(event.mouseMove.x, event.mouseMove.y);
        const Vec2 pixel2f = window.mapPixelToCoords(mouse_position);

        if (_shape.getGlobalBounds().contains(pixel2f)) {
            _shape.setFillColor(sf::Color(200, 200, 200, 150));
        } else {
            _shape.setFillColor(sf::Color::White);
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

        const sf::Vector2i click_position(event.mouseButton.x, event.mouseButton.y);
        const Vec2 pixel2f = window.mapPixelToCoords(click_position);

        if (_shape.getGlobalBounds().contains(pixel2f) && _onClick) {
            _onClick();
        }
    }
}
