/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IWidget.hpp
*/

#pragma once

#include "IWidget.hpp"
#include "SFMLMacros.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

namespace raytracer::ui {

class Button final : public IWidget
{
    public:
        explicit Button(const Vec2 &position, const Vec2 &size, const std::string &label, sf::Font &font,
            const uint fontSize = 24);

        void setOnClick(std::function<void()> callback);

        void render(sf::RenderWindow &window) noexcept override;
        void update(UNUSED const float dt) noexcept override;
        void onEvent(const sf::Event &event, const sf::RenderWindow &window) noexcept override;

    private:
        sf::Text _text;
        sf::RectangleShape _shape;
        std::function<void()> _onClick;
};
}// namespace raytracer::ui
