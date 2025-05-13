/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Application.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace raytracer::core {
class Application final
{
    public:
        explicit Application();
        ~Application() = default;

        void run();

    private:
        void setupUI();

        sf::RenderWindow _window;
};
}// namespace raytracer::core
