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

static void createText(const std::string &str, const raytracer::Vec2 &pos, sf::Font &font, sf::Text &text,
    const uint fontSize = 24)
{
    text.setPosition(pos);
    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::Black);
    text.setString(str);
}

raytracer::ui::Text::Text(const std::string &str, const Vec2 &pos, sf::Font &font, const uint fontSize) noexcept
{
    createText(str, pos, font, _text, fontSize);
}

void raytracer::ui::Text::render(sf::RenderWindow &window) noexcept
{
    window.draw(_text);
}

void raytracer::ui::Text::update(const float UNUSED dt) noexcept
{
    /* empty */
}

void raytracer::ui::Text::onEvent(const sf::Event UNUSED &event, const sf::RenderWindow UNUSED &window) noexcept
{
    /* empty */
}
