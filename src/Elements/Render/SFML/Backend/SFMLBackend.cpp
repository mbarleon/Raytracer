/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SFMLBackend.cpp
*/

#include "SFMLBackend.hpp"
#include "Error.hpp"
#include "SFMLBackendMacros.hpp"

/**
* public
*/

raytracer::backend::SFMLBackend::SFMLBackend()
{
    _init();
}

raytracer::backend::SFMLBackend::~SFMLBackend()
{
    _stop();
}

/**
* private
*/

static void create_window(sf::RenderWindow &window)
{
    sf::ContextSettings settings;

    settings.antialiasingLevel = DEFAULT_ANTIALIASING;
    settings.majorVersion = OPENGL_MAJOR_VERSION;
    settings.minorVersion = OPENGL_MINOR_VERSION;

    window.create(DEFAULT_VIDEOMODE, "Raytracer", sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar, settings);
    window.setFramerateLimit(DEFAULT_FRAMERATE);
    window.setVerticalSyncEnabled(DEFAULT_VSYNC);
}

/**
* @brief _init (called by __ctor__)
* @details initialize the backend
* @return void
*/
void raytracer::backend::SFMLBackend::_init()
{
    create_window(_window);
}

/**
* @brief _stop (called by __dtor__)
* @details stop the backend
* @return void
*/
void raytracer::backend::SFMLBackend::_stop()
{
    _window.close();
}
