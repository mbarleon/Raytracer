/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UITextInput.cpp
*/

#include "UITextInput.hpp"
#include "SFMLMacros.hpp"

/**
* public
*/

// clang-format off

static void createText(const raytracer::Vec2 &pos,sf::Font &font, sf::Text &text, const uint fontSize = 24)
{
    text.setPosition(pos);
    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::Black);
}

raytracer::ui::TextInput::TextInput(const Vec2 &pos, sf::Font &font, CallbackStr callback, const uint fontSize) noexcept
{
    createText({pos.x, pos.y + 32}, font, _text, fontSize);
    _callback = callback;
}

void raytracer::ui::TextInput::render(sf::RenderWindow &window) noexcept
{
    window.draw(_text);
}

void raytracer::ui::TextInput::update(const float UNUSED dt) noexcept
{
    /* empty */
}

void raytracer::ui::TextInput::onEvent(const sf::Event& event, const sf::RenderWindow UNUSED &window) noexcept
{
    static sf::Uint32 last_char = 0;
    static bool was_pressed = false;

    switch (event.type) {
        case sf::Event::TextEntered: {
            const sf::Uint32 unicode = event.text.unicode;

            if (unicode == BACKSPACE) {
                if (!_buffer.empty() && (was_pressed || last_char != BACKSPACE)) {
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
            break;
        }
        case sf::Event::KeyReleased: {
            was_pressed = false;
            last_char = DEFAULT; 
            break;
        }
        default:
            break;
    }
}

/**
* private
*/

// clang-format on
