/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Backend.hpp
*/

#pragma once

#define RT_SFML_USE_INLINES
#include "SFMLMacros.hpp"
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
        void exportScene(CallbackStr callback) noexcept;

        [[nodiscard]] bool is_running() const noexcept;
        [[nodiscard]] float getDeltaTime() noexcept;
        [[nodiscard]] const std::optional<sf::Event> event() noexcept;
        [[nodiscard]] sf::RenderWindow &getWindow() noexcept;

    private:
        sf::Clock _clock;
        sf::RenderWindow _window;

        void update() noexcept;
        void close(sf::RenderWindow &window) noexcept;
        [[nodiscard]] const std::optional<sf::Event> event_logic(sf::RenderWindow &window) noexcept;
};
}// namespace raytracer::core
