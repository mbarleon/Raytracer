/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIText.cpp
*/

#include "UIText.hpp"

/**
* public
*/

raytracer::ui::Text::Text(const std::string &str, const Vec2 &pos, sf::Font &font, const uint fontSize) noexcept
    : _text(font, str, fontSize)
{
    _text.setPosition(pos);
    _text.setFillColor(sf::Color::Black);
}

void raytracer::ui::Text::render(sf::RenderWindow &window) noexcept
{
    window.draw(_text);
}

void raytracer::ui::Text::update(const float UNUSED dt) noexcept
{
    /* empty */
}

void raytracer::ui::Text::onEvent(const std::optional<sf::Event> UNUSED &event, const sf::RenderWindow UNUSED &window) noexcept
{
    /* empty */
}
