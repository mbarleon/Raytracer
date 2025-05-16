/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIText.hpp
*/

#pragma once

#include "IWidget.hpp"
#include "SFMLMacros.hpp"
#include <SFML/Graphics/Text.hpp>

namespace raytracer::ui {

class Text final : public IWidget
{
    public:
        explicit Text(const std::string &str, const Vec2 &pos, sf::Font &font, const uint fontSize) noexcept;

        void render(sf::RenderWindow &window) noexcept override;
        void update(const float dt) noexcept override;
        void onEvent(const sf::Event &event, const sf::RenderWindow &window) noexcept override;

    private:
        sf::Text _text;
};
}// namespace raytracer::ui
