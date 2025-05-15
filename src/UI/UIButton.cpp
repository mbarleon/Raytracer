/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIButton.cpp
*/

#include "UIButton.hpp"

raytracer::ui::Button::Button(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &label, sf::Font &font,
    const uint fontSize)
{
    _shape.setPosition(position);
    _shape.setSize(size);
    _shape.setFillColor(sf::Color::White);

    _text.setFont(font);
    _text.setString(label);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(sf::Color::Black);
    _text.setPosition(position + sf::Vector2f(10, 10));
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
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

        const auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

        if (_shape.getGlobalBounds().contains(mousePos) && _onClick) {
            _onClick();
        }
    }
}
