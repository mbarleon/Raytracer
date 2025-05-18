/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_SFMLMacros.cpp
*/

#define RT_SFML_USE_INLINES
#include "SFMLMacros.hpp"
#include <criterion/criterion.h>

Test(_to_vec2f, to_vec2f)
{
    const sf::Vector2u vec2u = {1, 2};
    const sf::Vector2f vec = raytracer::ui::_to_vec2f(vec2u);

    cr_assert_eq(vec.x, 1);
    cr_assert_eq(vec.y, 2);
}

#include <iostream>
Test(_get_scale, get_scale)
{
    const sf::Vector2f vec = {1, 2};
    const sf::Vector2f scale = raytracer::ui::_get_scale(vec);

    cr_assert_eq(scale.x, 1);
    cr_assert_eq(scale.y, 1);
}

Test(_get_scale, get_scale_if)
{
    const sf::Vector2f vec = {1920, 1080};
    const sf::Vector2f scale = raytracer::ui::_get_scale(vec);

    cr_assert_float_eq(scale.x, 0.6666667, 0.000001);
}

Test(_center_sprite, center_sprite)
{
    sf::RenderWindow window;
    sf::Sprite sprite;

    window.setSize({1920, 1080});
    sprite.setPosition({123, 121});
    raytracer::ui::_center_sprite(window, sprite);
    const sf::Vector2f pos = sprite.getPosition();
    cr_assert_eq(pos.x, 0);
    cr_assert_eq(pos.y, 0);
}
