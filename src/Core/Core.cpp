/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include "../Parser/Parser.hpp"
#include "CoreFactory.hpp"
#include <unordered_map>

/*
* public
*/

/**
 * @brief Core run
 * @details Core entry-point, starts the raytracer program
 * @return void
 */
void raytracer::Core::run(const char *RESTRICT filename)
{
    const parser::JsonValue jsonc = parser::parseJsonc(filename);
    const auto &root = std::get<JsonMap>(jsonc);
    const auto &render = root.at("render");
    const auto &camera = root.at("camera");
    const auto &scene = std::get<JsonMap>(root.at("scene").value);
    const auto &primitives = scene.at("primitives");

    _materials = material_factory(root.at("scene"));
    _shapes = primitive_factory(primitives, _materials);
    _render = create_render(render);
    _camera = create_camera(camera);
    _camera.get()->render(_shapes, *_render.get());
}
