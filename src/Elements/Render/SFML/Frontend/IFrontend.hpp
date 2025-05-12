/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IFrontend.hpp
*/

#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

namespace raytracer::frontend {

class IFrontend
{
    public:
        virtual ~IFrontend() = default;

        virtual void render(sf::RenderWindow &window) const noexcept = 0;
        virtual void update(const sf::Event event) noexcept = 0;

        [[nodiscard]] virtual bool shouldRender() const noexcept = 0;
};

}// namespace raytracer::frontend
