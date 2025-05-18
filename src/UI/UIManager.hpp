/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIManager.hpp
*/

#pragma once

#include "Logger.hpp"
#include "SFMLMacros.hpp"
#include "UIButton.hpp"
#include "UIContainer.hpp"
#include "UIRectangle.hpp"
#include "UIText.hpp"
#include "UITextInput.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace raytracer::ui {

using ButtonPtr = std::shared_ptr<Button>;
using RectanglePtr = std::shared_ptr<Rectangle>;
using TextPtr = std::shared_ptr<Text>;
using TextInputPtr = std::shared_ptr<TextInput>;

/**
* @brief default debug callback for buttons
*/
static inline void _clicked()
{
    logger::debug("clicked!");
}

// clang-format off

/**
* @class UIManager
* @brief class to manage the UI components and events.
*/
class UIManager final
{
    public:
        explicit UIManager(sf::RenderWindow &window);
        ~UIManager() = default;

        [[nodiscard]] Container &getContainer() noexcept;
        [[nodiscard]] RectanglePtr createRectangle(const Vec2 &pos, const Vec2 &size);
        [[nodiscard]] ButtonPtr createButton(const std::string &text, const Vec2 &pos, const Vec2 &size, Callback callback = _clicked, const uint fontSize = 24) noexcept;
        [[nodiscard]] TextPtr createText(const std::string &str, const Vec2 &pos,  uint fontSize = 24) noexcept;
        [[nodiscard]] TextInputPtr createTextInput(const Vec2 &pos, CallbackStr callback, const uint fontSize = 24) noexcept;

        void events(const std::optional<sf::Event> &event) noexcept;
        void update(const float dt) noexcept;
        void render() noexcept;

        /* TODO: set theme | get theme */

    private:
        void initialize(sf::RenderWindow &window);

        sf::Font _font;
        sf::RenderWindow *_window = nullptr;
        ContainerPtr _container = nullptr;
};
// clang-format on
}// namespace raytracer::ui
