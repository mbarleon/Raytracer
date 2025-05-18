/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIRectangle.hpp
*/

#pragma once

#include "IWidget.hpp"
#include "SFMLMacros.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace raytracer::ui {

class Rectangle final : public IWidget
{
    public:
        explicit Rectangle(const Vec2 &position, const Vec2 &size);

        void render(sf::RenderWindow &window) noexcept override;
        void update(const float dt) noexcept override;
        void onEvent(const std::optional<sf::Event> &event, const sf::RenderWindow &window) noexcept override;

    private:
        sf::RectangleShape _shape;
};
}// namespace raytracer::ui
