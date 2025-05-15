/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UITextInput.hpp
*/

#pragma once

#include "IWidget.hpp"
#include "SFMLMacros.hpp"
#include <SFML/Graphics/Text.hpp>

namespace raytracer::ui {

class TextInput final : public IWidget
{
    public:
        explicit TextInput(const Vec2 &pos, sf::Font &font, CallbackStr callback, const uint fontSize = 24) noexcept;

        void render(sf::RenderWindow &window) noexcept override;
        void update(const float dt) noexcept override;
        void onEvent(const sf::Event &event, const sf::RenderWindow &window) noexcept override;

    private:
        sf::Text _text;
        CallbackStr _callback;
        std::string _buffer;

        // clang-format off
        enum UNICODE {
            DEFAULT = 0,
            BACKSPACE = 8,
            ENTER = 13,
            ESCAPE = 27,
            SPACE = 32,
            DELETE = 127
        };
        // clang-format on
};
}// namespace raytracer::ui
