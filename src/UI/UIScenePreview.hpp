/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIScenePreview.hpp
*/

#pragma once

#include "IWidget.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace raytracer::ui {

#define MAX_PIXEL_BUFFER_WIDTH 1280
#define MAX_PIXEL_BUFFER_HEIGHT 720
#define DEFAULT_SCALE 1.f

using PixelBuffer = sf::Image;

/**
* @class UIScenePreview
* @brief Class for the scene preview in the UI.
* @pattern Composite desing pattern (concrete final class)
*/
class UIScenePreview final : public IWidget
{
    public:
        explicit UIScenePreview(const PixelBuffer &image);
        ~UIScenePreview() override = default;

        void render(sf::RenderWindow &window) noexcept override;
        void update(const float dt) noexcept override;
        void onEvent(const sf::Event &event, const sf::RenderWindow &window) noexcept override;

    private:
        PixelBuffer _buffer;
        sf::Texture _texture;
        sf::Sprite _sprite;
};

static inline const sf::Vector2f _to_vec2f(const sf::Vector2u &vec)
{
    return sf::Vector2f(static_cast<float>(vec.x), static_cast<float>(vec.y));
}

static inline const sf::Vector2f _get_scale(const sf::Vector2f &texture_size)
{
    const float scaleX = MAX_PIXEL_BUFFER_WIDTH / texture_size.x;
    const float scaleY = MAX_PIXEL_BUFFER_HEIGHT / texture_size.y;
    const float scale = std::min(scaleX, scaleY);

    if (scale < DEFAULT_SCALE) {
        return sf::Vector2f(scale, scale);
    }
    return sf::Vector2f(DEFAULT_SCALE, DEFAULT_SCALE);
}

static inline void _center_sprite(const sf::RenderWindow &window, sf::Sprite &sprite)
{
    const sf::Vector2f size = _to_vec2f(window.getSize());
    const sf::FloatRect bounds = sprite.getGlobalBounds();

    const float posX = (size.x - bounds.width) / 2.f;
    const float posY = (size.y - bounds.height) / 2.f;

    sprite.setPosition(posX, posY);
}

}// namespace raytracer::ui
