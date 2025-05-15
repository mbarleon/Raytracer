/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Backend.hpp
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace raytracer::core {

class Backend final
{
    public:
        explicit Backend();
        ~Backend() = default;

        void stop() noexcept;
        void fullscreen() noexcept;
        void exportScene() noexcept;

        [[nodiscard]] bool is_running() const noexcept;
        [[nodiscard]] float getDeltaTime() noexcept;
        [[nodiscard]] const sf::Event event() noexcept;
        [[nodiscard]] sf::RenderWindow &getWindow() noexcept;

    private:
        sf::Clock _clock;
        sf::RenderWindow _window;

        void update() noexcept;
        void close(sf::RenderWindow &window) noexcept;
        [[nodiscard]] const sf::Event event_logic(sf::RenderWindow &window) noexcept;
};
}// namespace raytracer::core
