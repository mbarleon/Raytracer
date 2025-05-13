/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIManager.cpp
*/

#include "UIManager.hpp"
#include "Error.hpp"

raytracer::ui::UIManager &raytracer::ui::UIManager::getInstance()
{
    static UIManager instance;

    return instance;
}

void raytracer::ui::UIManager::initialize(sf::RenderWindow &window)
{
    _window = &window;

    if (!_font.loadFromFile("assets/fonts/jetbrains.ttf")) {
        throw exception::Error("UIManager::initialize", "Failed to load font");
    }
    _container = std::make_shared<Container>();
}

void raytracer::ui::UIManager::events(const sf::Event &event)
{
    if (_window) {
        _container->events(event, *_window);
    }
}

void raytracer::ui::UIManager::update(float dt)
{
    if (_window) {
        _container->update(dt);
    }
}

void raytracer::ui::UIManager::render()
{
    if (_window) {
        _window->clear();
        _container->render(*_window);
        _window->display();
    }
}
