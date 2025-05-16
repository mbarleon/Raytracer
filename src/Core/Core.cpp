/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include "../Parser/Parser.hpp"
#include "../Elements/Scene/Shapes/IShape.hpp"
#include "CoreFactory.hpp"
#include <unordered_map>
#include "Error.hpp"

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

    try {
        const auto &root = std::get<JsonMap>(jsonc);
        const auto &render = root.at("render");
        const auto &camera = root.at("camera");
        const auto &scene = std::get<JsonMap>(root.at("scene").value);
        const auto &shapes = scene.at("shapes");
        const auto &lights = scene.at("lights");

        _shapes = primitive_factory(shapes);
        _lights = light_factory(lights);

        _render = create_render(render);
        try {
            _camera = create_camera(camera);
        } catch (const std::bad_alloc &e) {
            throw exception::Error("Core", "Camera bad allocation", e.what());
        }
        _camera->render(_shapes, _lights, _render);
    } catch (const std::out_of_range &e) {
        throw raytracer::exception::Error("Core", "Invalid configuration file", e.what());
    }
}
