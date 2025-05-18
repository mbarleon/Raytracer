/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UITextInput.cpp
*/

#include "UITextInput.hpp"
#include "SFMLMacros.hpp"
#include <variant>

/**
* public
*/

// clang-format off

raytracer::ui::TextInput::TextInput(const Vec2 &pos, sf::Font &font, CallbackStr callback, const uint fontSize) noexcept
: _text(font, "", fontSize), _callback(callback)
{
    _text.setPosition({pos.x, pos.y + 32.f});
    _text.setFillColor(sf::Color::Black);
}

void raytracer::ui::TextInput::render(sf::RenderWindow &window) noexcept
{
    window.draw(_text);
}

void raytracer::ui::TextInput::update(const float UNUSED dt) noexcept
{
    /* empty */
}

void raytracer::ui::TextInput::onEvent(const std::optional<sf::Event>& event, const sf::RenderWindow UNUSED &window) noexcept
{
    static u32 last_char = 0;
    static bool was_pressed = false;

    if (!event.has_value())
        return;

    if (event->is<sf::Event::FocusGained>()) {
        return;
    }

    if (event->is<sf::Event::TextEntered>()) {
        const auto* textEvent = event->getIf<sf::Event::TextEntered>();
        if (!textEvent)
            return;

        const u32 unicode = textEvent->unicode;

        if (unicode == BACKSPACE) {
            if (!_buffer.empty() && (!was_pressed || last_char != BACKSPACE)) {
                _buffer.pop_back();
                _text.setString(_buffer);
                last_char = BACKSPACE;
                was_pressed = true;
            }
            return;
        }
        else if (unicode == ENTER) {
            _callback(_buffer);
            return;
        }

        if (unicode >= SPACE && unicode < DELETE) {
            if (!was_pressed || unicode != last_char) {
                _buffer += static_cast<char>(unicode);
                _text.setString(_buffer);
                last_char = unicode;
                was_pressed = true;
            }
        }

        return;
    }

    if (event->is<sf::Event::KeyReleased>()) {
        was_pressed = false;
        last_char = DEFAULT;
        return;
    }
}

/**
* private
*/

// clang-format on
