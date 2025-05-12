/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ABackend.hpp
*/

#pragma once

#include "IBackend.hpp"

namespace raytracer::backend {

class ABackend : public IBackend
{
    public:
        explicit ABackend() = default;
        virtual ~ABackend() = default;

        void clear() noexcept override;
        void display() noexcept override;
        const sf::Event getEvents() noexcept override;

        [[nodiscard]] bool isOpen() const noexcept override;
        [[nodiscard]] sf::RenderWindow &getWindow() noexcept override;
        [[nodiscard]] bool getAction(const float every, float &last) noexcept override;

    protected:
        sf::RenderWindow _window;
        sf::Clock _clock;
};
}// namespace raytracer::backend
