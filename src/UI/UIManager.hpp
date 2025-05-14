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

using ContainerPtr = std::shared_ptr<Container>;

/**
* @class UIManager
* @brief Singleton class to manage the UI components and events.
*/
class UIManager final
{
    public:
        [[nodiscard]] static UIManager &getInstance() noexcept;
        [[nodiscard]] Container &getContainer() noexcept;
        [[nodiscard]] sf::Font &getFont() noexcept;

        void initialize(sf::RenderWindow &window);
        void events(const sf::Event &event) noexcept;
        void update(const float dt) noexcept;
        void render() noexcept;

        /* TODO: set theme | get theme */

    private:
        explicit UIManager() = default;
        ~UIManager() = default;

        constexpr UIManager(const UIManager &) = delete;
        constexpr UIManager &operator=(const UIManager &) = delete;
        constexpr UIManager(UIManager &&) = delete;
        constexpr UIManager &operator=(UIManager &&) = delete;

        sf::Font _font;
        sf::RenderWindow *_window = nullptr;
        ContainerPtr _container = nullptr;
};
}// namespace raytracer::ui
