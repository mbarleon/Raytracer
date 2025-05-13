/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Application.cpp
*/

#include "Application.hpp"
#include "../UI/UIButton.hpp"
#include "../UI/UIManager.hpp"
#include <iostream>

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
    sf::Clock clock;

    while (_window.isOpen()) {

        sf::Event event{};

        while (_window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                _window.close();

            ui::UIManager::getInstance().events(event);
        }

        float dt = clock.restart().asSeconds();
        ui::UIManager::getInstance().update(dt);
        ui::UIManager::getInstance().render();
    }
}

/**
* private
*/

void raytracer::core::Application::setupUI()
{
    // ui::UIManager &ui = ui::UIManager::getInstance();
    // ui::Container &container = ui.getContainer();

    // auto button =
    //     std::make_shared<ui::Button>(sf::Vector2f(50.f, 50.f), sf::Vector2f(200.f, 50.f), std::string("Click me!"), ui.getFont());

    // button->setOnClick([]() { std::cout << "Button clicked!" << std::endl; });
    // container.addWidget(button);
}
