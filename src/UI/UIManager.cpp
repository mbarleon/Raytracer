/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIManager.cpp
*/

#include "UIManager.hpp"
#include "Error.hpp"
#include "Events/EventManager.hpp"

raytracer::ui::UIManager &raytracer::ui::UIManager::getInstance() noexcept
{
    static UIManager instance;

    return instance;
}

raytracer::ui::Container &raytracer::ui::UIManager::getContainer() noexcept
{
    return *_container;
}

sf::Font &raytracer::ui::UIManager::getFont() noexcept
{
    return _font;
}

void raytracer::ui::UIManager::initialize(sf::RenderWindow &window)
{
    _window = &window;

    if (!_font.loadFromFile("assets/fonts/jetbrains.ttf")) {
        throw exception::Error("UIManager::initialize", "Failed to load font");
    }
    _container = std::make_shared<Container>();
    EventManager::getInstance().subscribe(_container);
}

void raytracer::ui::UIManager::events(const sf::Event &event) noexcept
{
    if (_window) {
        _container->onEvent(event, *_window);
    }
}

void raytracer::ui::UIManager::update(const float dt) noexcept
{
    if (_window) {
        _container->update(dt);
    }
}

void raytracer::ui::UIManager::render() noexcept
{
    if (_window) {
        _window->clear();
        _container->render(*_window);
        _window->display();
    }
}
