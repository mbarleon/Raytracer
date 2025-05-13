/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IWidget.hpp
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace raytracer::ui {

/**
* @class IWidget
* @brief Interface for all UI widgets.
* @pattern Composite desing pattern (interface)
*/
class IWidget
{
    public:
        virtual ~IWidget() = default;

        virtual void render(sf::RenderWindow &window) noexcept = 0;
        virtual void update(const float dt) noexcept = 0;
        virtual void events(const sf::Event &event, const sf::RenderWindow &window) noexcept = 0;
};
}// namespace raytracer::ui
