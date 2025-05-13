/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIManager.hpp
*/

#pragma once

#include "UIContainer.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

namespace raytracer::ui {

/**
* @class UIManager
* @brief Singleton class to manage the UI components and events.
*/
class UIManager final
{
    public:
        static UIManager &getInstance();

        void initialize(sf::RenderWindow &window);
        void events(const sf::Event &event);
        void update(float dt);
        void render();

        /* TODO: set theme | get theme */

    private:
        UIManager() = default;
        ~UIManager() = default;

        UIManager(const UIManager &) = delete;
        UIManager &operator=(const UIManager &) = delete;
        UIManager(UIManager &&) = delete;
        UIManager &operator=(UIManager &&) = delete;

        sf::Font _font;
        sf::RenderWindow *_window = nullptr;
        std::shared_ptr<Container> _container;
};
}// namespace raytracer::ui
