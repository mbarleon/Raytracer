/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIManager.cpp
*/

#include "UIManager.hpp"
#include "Error.hpp"
#include "Events/EventManager.hpp"
#include "SFMLMacros.hpp"

/**
* public
*/

raytracer::ui::UIManager::UIManager(sf::RenderWindow &window)
{
    initialize(window);
}

// clang-format off
raytracer::ui::ButtonPtr raytracer::ui::UIManager::createButton(const std::string &text, const Vec2 &pos, const Vec2 &size, Callback callback, const uint fontSize) noexcept
{
    ButtonPtr button = std::make_shared<Button>(pos, size, text, _font, fontSize);

    button->setOnClick(callback);
    return button;
}

raytracer::ui::RectanglePtr raytracer::ui::UIManager::createRectangle(const Vec2 &pos, const Vec2 &size)
{
    return std::make_shared<Rectangle>(pos, size);
}
// clang-format on

raytracer::ui::Container &raytracer::ui::UIManager::getContainer() noexcept
{
    return *_container;
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
        _window->clear(sf::Color::White);
        _container->render(*_window);
        _window->display();
    }
}

/**
* private
*/

void raytracer::ui::UIManager::initialize(sf::RenderWindow &window)
{
    _window = &window;

    if (!_font.loadFromFile(RT_DEFAULT_FONT_PATH)) {
        throw exception::Error("UIManager::initialize", "Failed to load font");
    }
    _container = std::make_shared<Container>();
    EventManager::getInstance().subscribe(_container);
}
