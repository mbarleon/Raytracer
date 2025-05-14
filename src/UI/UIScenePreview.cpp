/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIScenePreview.cpp
*/

#include "UIScenePreview.hpp"

raytracer::ui::UIScenePreview::UIScenePreview(const PixelBuffer &image)
{
    _texture.loadFromImage(image);
    _sprite.setTexture(_texture);
    _sprite.setScale(_get_scale(sf::Vector2f(_texture.getSize())));
}

void raytracer::ui::UIScenePreview::render(sf::RenderWindow &window) noexcept
{
    static bool is_first = true;

    if (is_first) {
        _center_sprite(window, _sprite);
    }

    window.draw(_sprite);
}

void raytracer::ui::UIScenePreview::update(const float __attribute__((unused)) dt) noexcept
{
    //
}

void raytracer::ui::UIScenePreview::events(const sf::Event __attribute__((unused)) & event,
    const sf::RenderWindow __attribute__((unused)) & window) noexcept
{
    //
}
