/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** EventManager.cpp
*/

#include "EventManager.hpp"
#include "IEventObserver.hpp"

/*
* public
*/

raytracer::ui::EventManager &raytracer::ui::EventManager::getInstance() noexcept
{
    static EventManager instance;

    return instance;
}

/**
* @brief EventManager::EventManager
* @details constructor
* @return void
*/
void raytracer::ui::EventManager::subscribe(const EventDecorator &observer) noexcept
{
    _observers.push_back(observer);
}

/**
* @brief EventManager::unsubscribe
* @details unsubscribes an observer from the event manager.
* @return void
*/
void raytracer::ui::EventManager::unsubscribe(const EventDecorator &observer) noexcept
{
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}

/**
* @brief EventManager::dispatch
* @details dispatches an event to all observers.
* @return void
*/
void raytracer::ui::EventManager::dispatch(const sf::Event &event, const sf::RenderWindow &window) noexcept
{
    for (const auto &observer : _observers) {

        if (observer) {
            observer->onEvent(event, window);
        }
    }
}
