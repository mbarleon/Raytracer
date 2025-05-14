/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIScenePreview.cpp
*/

#include "UIScenePreview.hpp"
#include "../Core/CoreFactory.hpp"
#include "../Parser/Parser.hpp"

raytracer::ui::UIScenePreview::UIScenePreview(const char *RESTRICT filename)
{
    const parser::JsonValue jsonc = parser::parseJsonc(filename);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &scene = std::get<JsonMap>(root.at("scene").value);
    const auto &shapes = scene.at("shapes");
    const auto &lights = scene.at("lights");

    _shapes = primitive_factory(shapes);
    _lights = light_factory(lights);
}

void raytracer::ui::UIScenePreview::render(sf::RenderWindow __attribute__((unused)) & window) noexcept
{
    //
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
