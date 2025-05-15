/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIManager.hpp
*/

#pragma once

#include "SFMLMacros.hpp"
#include "UIContainer.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace raytracer::ui {

/**
* @class UIManager
* @brief class to manage the UI components and events.
*/
class UIManager final
{
    public:
        UIManager(sf::RenderWindow &window);

        [[nodiscard]] Container &getContainer() noexcept;
        [[nodiscard]] sf::Font &getFont() noexcept;

        void initialize(sf::RenderWindow &window);
        void events(const sf::Event &event) noexcept;
        void update(const float dt) noexcept;
        void render() noexcept;

        /* TODO: set theme | get theme */

    private:
        sf::Font _font;
        sf::RenderWindow *_window = nullptr;
        ContainerPtr _container = nullptr;
};
}// namespace raytracer::ui
