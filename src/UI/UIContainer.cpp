/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIContainer.cpp
*/

#include "UIContainer.hpp"

/**
* public
*/

void raytracer::ui::Container::addWidget(const std::shared_ptr<IWidget> widget) noexcept
{
    _children.push_back(widget);
}

void raytracer::ui::Container::removeWidget(std::shared_ptr<IWidget> widget) noexcept
{
    const auto it = std::remove(_children.begin(), _children.end(), widget);

    if (it != _children.end())
        _children.erase(it, _children.end());
}

void raytracer::ui::Container::update(const float dt) noexcept
{
    for (const auto &child : _children) {
        child->update(dt);
    }
}

void raytracer::ui::Container::render(sf::RenderWindow &window) noexcept
{
    for (const auto &child : _children) {
        child->render(window);
    }
}

void raytracer::ui::Container::onEvent(const std::optional<sf::Event> &event, const sf::RenderWindow &window) noexcept
{
    for (const auto &child : _children) {
        child->onEvent(event, window);
    }
}
