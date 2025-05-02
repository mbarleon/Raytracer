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
    const auto &root = std::get<std::unordered_map<std::string, parser::JsonProto>>(jsonc);
    const auto &scene = std::get<std::unordered_map<std::string, parser::JsonProto>>(root.at("scene").value);
    const auto &primitives = scene.at("primitives");
    const auto &camera = scene.at("camera");

    _shapes = primitive_factory(primitives);
    _camera = create_camera(camera);
    _camera.get()->render(_shapes);
}
