/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IEventObserver.hpp
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace raytracer::ui {

/**
* @class IEventObserver
* @pattern Observer design pattern (interface)
* @details listen to events from the EventManager
*
* @note works like a python decorator
*/
class IEventObserver
{
    public:
        virtual ~IEventObserver() = default;

        virtual void onEvent(const std::optional<sf::Event> &event, const sf::RenderWindow &window) noexcept = 0;
};

}// namespace raytracer::ui
