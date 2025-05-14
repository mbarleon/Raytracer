/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Application.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "Logger.hpp"
#include "Macro.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

namespace raytracer::core {

using Vec2 = sf::Vector2f;
using Callback = std::function<void()>;

/**
* @class Application
* @brief Main Core entry-point class for the raytracer application.
*/
class Application final
{
    public:
        explicit Application(const char *RESTRICT filename);
        ~Application() = default;

        void run();

    private:
        void setupUI();
        void setupPreview(const char *RESTRICT filename);

        void stop();
        void fullscreen();

        sf::RenderWindow _window;
        std::unique_ptr<Camera> _camera;
};

/**
* @brief default debug callback for buttons
*/
static inline void _clicked()
{
    logger::debug("clicked!");
}
}// namespace raytracer::core
