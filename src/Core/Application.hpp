/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Application.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "Macro.hpp"
#include <SFML/Graphics.hpp>

namespace raytracer::core {
class Application final
{
    public:
        explicit Application(const char *RESTRICT filename);
        ~Application() = default;

        void run();

    private:
        void setupUI();
        void setupPreview(const char *RESTRICT filename);

        sf::RenderWindow _window;
        std::unique_ptr<Camera> _camera;
};
}// namespace raytracer::core
