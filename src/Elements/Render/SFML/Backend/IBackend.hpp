/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IBackend.hpp
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace raytracer::backend {
class IBackend
{
    public:
        virtual ~IBackend() = default;

        virtual void clear() noexcept = 0;
        virtual void display() noexcept = 0;
        virtual const sf::Event getEvents() noexcept = 0;

        [[nodiscard]] virtual sf::RenderWindow &getWindow() noexcept = 0;
        [[nodiscard]] virtual bool getAction(const float every, float &last) noexcept = 0;
};
}// namespace raytracer::backend
