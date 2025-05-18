/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIButton.cpp
*/

#include "UIButton.hpp"

raytracer::ui::Button::Button(const Vec2 &position, const Vec2 &size, const std::string &label, sf::Font &font,
    const uint fontSize)
    : _text(font, label, fontSize)
{
    _shape.setPosition(position);
    _shape.setSize(size);
    _shape.setFillColor(sf::Color::White);

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

void raytracer::ui::Button::onEvent(const std::optional<sf::Event> &event, const sf::RenderWindow &window) noexcept
{
    if (!event.has_value()) {
        return;
    }

    if (event->is<sf::Event::MouseMoved>()) {
        const auto *mouse_event = event->getIf<sf::Event::MouseMoved>();

        if (!mouse_event) {
            return;
        }

        const sf::Vector2i mouse_pos(mouse_event->position.x, mouse_event->position.y);
        const sf::Vector2f world_pos = window.mapPixelToCoords(mouse_pos);

        if (_shape.getGlobalBounds().contains(world_pos)) {
            _shape.setFillColor(sf::Color(200, 200, 200, 150));
        } else {
            _shape.setFillColor(sf::Color::White);
        }
    }

    if (event->is<sf::Event::MouseButtonPressed>()) {

        const auto *mouse_event = event->getIf<sf::Event::MouseButtonPressed>();

        if (!mouse_event || mouse_event->button != sf::Mouse::Button::Left) {
            return;
        }

        const sf::Vector2i click_pos(mouse_event->position.x, mouse_event->position.y);
        const sf::Vector2f world_pos = window.mapPixelToCoords(click_pos);

        if (_shape.getGlobalBounds().contains(world_pos) && _onClick) {
            _onClick();
        }
    }
}
