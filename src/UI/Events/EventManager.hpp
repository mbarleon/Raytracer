/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** EventManager.hpp
*/

#pragma once

#include "IEventObserver.hpp"
#include "NonCopyable.hpp"
#include <memory>
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

        void subscribe(const std::shared_ptr<IEventObserver> &observer) noexcept;
        void unsubscribe(const std::shared_ptr<IEventObserver> &observer) noexcept;
        void dispatch(const sf::Event &event, const sf::RenderWindow &window) noexcept;

    private:
        constexpr explicit EventManager() = default;
        constexpr ~EventManager() = default;

        EventManager(const EventManager &) = delete;
        EventManager &operator=(const EventManager &) = delete;
        EventManager(EventManager &&) = delete;

        std::vector<std::shared_ptr<IEventObserver>> _observers;
};

}// namespace raytracer::ui
