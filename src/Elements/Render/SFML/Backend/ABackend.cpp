/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ABackend.cpp
*/

#include "ABackend.hpp"

/**
 * @brief getWindow
 * @details get the window pointer
 * @return window pointer
 */
sf::RenderWindow &raytracer::backend::ABackend::getWindow() noexcept
{
    return _window;
}

/**
 * @brief getAction
 * @details check if the elapsed time since the last action is greater than the given time
 * @return true <|> false
 */
bool raytracer::backend::ABackend::getAction(const float every, float &last_action) noexcept
{
    const float elapsed = _clock.getElapsedTime().asSeconds();

    if (elapsed - last_action >= every) {
        last_action = elapsed;
        return true;
    }
    return false;
}

/**
* @brief clear
* @details clear the window
*/
void raytracer::backend::ABackend::clear() noexcept
{
    _window.clear();
}

/**
* @brief display
* @details display the window
*/
void raytracer::backend::ABackend::display() noexcept
{
    _window.display();
}

/**
* @brief getEvents
* @details get the events from the window
* @retrun event (sf::Event)
*/
const sf::Event raytracer::backend::ABackend::getEvents() noexcept
{
    sf::Event event;

    while (_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                _window.close();
                break;
            case sf::Event::Resized:
                _window.setSize(sf::Vector2u(event.size.width, event.size.height));
                break;
            default:
                return event;
        }
    }
    return {};
}

/**
* @brief isOpen
* @details check if the window is open
* @return true <|> false
*/
bool raytracer::backend::ABackend::isOpen() const noexcept
{
    return _window.isOpen();
}
