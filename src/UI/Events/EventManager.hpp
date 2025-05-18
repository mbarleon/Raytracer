/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** EventManager.hpp
*/

#pragma once

#include "NonCopyable.hpp"
#include "SFMLMacros.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

namespace raytracer::ui {

/**
* @class EventManager
* @pattern Singleton (for the decorator callbacks)
*/
class EventManager final : public NonCopyable
{
    public:
        static EventManager &getInstance() noexcept;

        void subscribe(const EventDecorator &observer) noexcept;
        void unsubscribe(const EventDecorator &observer) noexcept;
        void dispatch(const std::optional<sf::Event> &event, const sf::RenderWindow &window) noexcept;

    private:
        std::vector<EventDecorator> _observers;
};

}// namespace raytracer::ui
