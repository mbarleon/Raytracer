/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Application.cpp
*/

#include "Application.hpp"
#include "../UI/UIManager.hpp"

/**
 * public
 */

raytracer::core::Application::Application()
{
    sf::ContextSettings settings;

    settings.antialiasingLevel = 8;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    _window.create({1920, 1080}, "Raytracer", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);
    _window.setFramerateLimit(60);

    ui::UIManager::getInstance().initialize(_window);

    setupUI();
}

void raytracer::core::Application::run()
{
}

/**
* private
*/

void raytracer::core::Application::setupUI()
{
}
