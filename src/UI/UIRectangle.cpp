/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIRectangle.cpp
*/

#include "UIRectangle.hpp"

/**
* public
*/

raytracer::ui::Rectangle::Rectangle(const Vec2 &position, const Vec2 &size)
{
    sf::RectangleShape rect(size);

    rect.setPosition(position);
    rect.setFillColor(sf::Color::White);
    rect.setOutlineThickness(3);
    rect.setOutlineColor(sf::Color::Black);
    _shape = rect;
}

void raytracer::ui::Rectangle::render(sf::RenderWindow &window) noexcept
{
    window.draw(_shape);
}

void raytracer::ui::Rectangle::update(const float UNUSED dt) noexcept
{
    /* empty */
}

void raytracer::ui::Rectangle::onEvent(const sf::Event UNUSED &event, const sf::RenderWindow UNUSED &window) noexcept
{
    /* empty */
}
