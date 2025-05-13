/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IWidget.hpp
*/

#pragma once

#include "IWidget.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

namespace raytracer::ui {

class Button : public IWidget
{
    public:
        explicit Button(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &label, sf::Font &font);

        void setOnClick(std::function<void()> callback);

        void render(sf::RenderWindow &window) noexcept override;
        void update(float dt) noexcept override;
        void events(const sf::Event &event, const sf::RenderWindow &window) noexcept override;

    private:
        sf::Text _text;
        sf::RectangleShape _shape;
        std::function<void()> _onClick;

        bool _hovered = false;
};
}// namespace raytracer::ui
