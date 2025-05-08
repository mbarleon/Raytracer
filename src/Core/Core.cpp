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
    std::vector<std::shared_ptr<raytracer::shape::IShape>> lights;
    std::vector<std::shared_ptr<raytracer::shape::IShape>> shapes;

    _materials = material_factory(root.at("scene"));
    _shapes = primitive_factory(primitives, _materials);

    for (const auto &obj : _shapes) {
        if (obj->getMaterial()->emissiveIntensity > 0.0) {
            lights.push_back(obj);
        } else {
            shapes.push_back(obj);
        }
    }

    _render = create_render(render);
    _camera = create_camera(camera);
    _camera.get()->render(shapes, lights, *_render.get());
}
