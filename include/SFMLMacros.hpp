/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SFMLMacros.hpp
*/

#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>

#if defined(RT_SFML_USE_INLINES)

    #include <SFML/Graphics/Sprite.hpp>
    #include <SFML/Graphics/Texture.hpp>

#endif

#include <memory>

/**
* @file SFMLMacros.hpp
* @brief macros and type definitions for SFML
*/

#define RT_DEFAULT_FONT_PATH "assets/fonts/jetbrains.ttf"

#define RT_MAX_PIXEL_BUFFER_WIDTH 1280
#define RT_MAX_PIXEL_BUFFER_HEIGHT 720
#define RT_DEFAULT_SCALE 1.f
#define RT_DEFAULT_SIZE {1920, 1080}
#define RT_DEFAULT_FPS 60

#define RT_DEFAULT_ANTIALIASING_LEVEL 8
#define RT_OPENGL_VERSION_MAJOR 4
#define RT_OPENGL_VERSION_MINOR 6
#define RT_DEFAULT_STENCILBITS 8
#define RT_DEFAULT_DEPTHBITS 24

#define RT_WINDOW_STYLE (sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)
#define RT_WINDOW_TITLE "Raytracer"

namespace raytracer {

using Vec2 = sf::Vector2f;
using PixelBuffer = sf::Image;

namespace ui {
class Container;
class IEventObserver;
using ContainerPtr = std::shared_ptr<Container>;
using EventDecorator = std::shared_ptr<IEventObserver>;

#if defined(RT_SFML_USE_INLINES)

static inline const Vec2 _to_vec2f(const sf::Vector2u &vec)
{
    return Vec2(static_cast<float>(vec.x), static_cast<float>(vec.y));
}

static inline const Vec2 _get_scale(const Vec2 &texture_size)
{
    const float scaleX = RT_MAX_PIXEL_BUFFER_WIDTH / texture_size.x;
    const float scaleY = RT_MAX_PIXEL_BUFFER_HEIGHT / texture_size.y;
    const float scale = std::min(scaleX, scaleY);

    if (scale < RT_DEFAULT_SCALE) {
        return Vec2(scale, scale);
    }
    return Vec2(RT_DEFAULT_SCALE, RT_DEFAULT_SCALE);
}

static inline void _center_sprite(const sf::RenderWindow &window, sf::Sprite &sprite)
{
    const Vec2 size = _to_vec2f(window.getSize());
    const sf::FloatRect bounds = sprite.getGlobalBounds();

    const float posX = (size.x - bounds.width) / 2.f;
    const float posY = (size.y - bounds.height) / 2.f;

    sprite.setPosition(posX, posY);
}

#endif

}// namespace ui
}// namespace raytracer
