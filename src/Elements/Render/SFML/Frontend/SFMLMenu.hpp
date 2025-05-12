/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SFMLMenu.hpp
*/

#pragma once

#include "AFrontend.hpp"

namespace raytracer::frontend {

class SFMLMenu final : public AFrontend
{
    public:
        explicit SFMLMenu() = default;
        ~SFMLMenu() override = default;

        void render(sf::RenderWindow &window) const noexcept override;
        void update(const sf::Event event) noexcept override;
};

}// namespace raytracer::frontend
