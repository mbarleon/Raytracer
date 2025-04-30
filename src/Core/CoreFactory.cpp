/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.cpp
*/

#include "CoreFactory.hpp"
#include "../Shapes/Rectangle.hpp"
#include "../Shapes/Sphere.hpp"
#include "Error.hpp"
#include "Macro.hpp"
#include <memory>

/**
* @brief
* @details private static
* @return
*/
unit_static double get_double(const ParsedJson &proto)
{
    if (std::holds_alternative<int>(proto.value)) {
        return static_cast<double>(std::get<int>(proto.value));
    }
    if (std::holds_alternative<double>(proto.value)) {
        return std::get<double>(proto.value);
    }
    throw raytracer::exception::Error("Core", "Expected number (int or double)");
}

/**
* @brief
* @details private static
* @return
*/
unit_static math::Vector3D get_vec3D(const ParsedJson &proto)
{
    const auto &obj = std::get<JsonMap>(proto.value);

    return math::Vector3D(get_double(obj.at("x")), get_double(obj.at("y")), get_double(obj.at("z")));
}

/**
* @brief
* @details private static
* @return
*/
/*unit_static std::shared_ptr<raytracer::shape::Sphere> get_material(const ParsedJson &proto)*/
/*{*/
/**/
/* TODO: later add material using pattern matching and lambda return class ptr*/
/*}*/

/**
* @brief
* @details private static
* @return
*/
unit_static std::shared_ptr<raytracer::shape::Sphere> create_spheres(const ParsedJson &proto)
{
    const auto &obj = std::get<JsonMap>(proto.value);
    const math::Vector3D position = get_vec3D(obj.at("position"));
    const double radius = get_double(obj.at("radius"));
    /*const auto &material = get_material(obj.at("material"));*/

    return std::make_shared<raytracer::shape::Sphere>(position, radius);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief
* @details private static
* @return
*/
unit_static std::shared_ptr<raytracer::shape::Rectangle> create_rectangle(const raytracer::parser::JsonProto &proto)
{
    const auto &obj = std::get<JsonMap>(proto.value);
    const math::Vector3D origin = get_vec3D(obj.at("origin"));
    const math::Vector3D bottom_side = get_vec3D(obj.at("bottom_side"));
    const math::Vector3D left_side = get_vec3D(obj.at("left_side"));
    /*const auto &material = get_material(obj.at("material"));*/

    return std::make_shared<raytracer::shape::Rectangle>(origin, bottom_side, left_side);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief
* @details private static
* @return
*/
unit_static math::Vector2u get_vec2u(const raytracer::parser::JsonProto &proto)
{
    const auto &obj = std::get<JsonMap>(proto.value);
    const double x = get_double(obj.at("width"));
    const double y = get_double(obj.at("height"));

    return math::Vector2u{static_cast<uint>(x), static_cast<uint>(y)};
}

/**
* @brief Entry Point
* @details Entry point
* @return TODO
*/
std::unique_ptr<raytracer::Camera> create_camera(const raytracer::parser::JsonProto &camera_json)
{
    const auto &obj = std::get<JsonMap>(camera_json.value);
    const math::Vector2u vec2 = get_vec2u(obj.at("resolution"));
    const math::Point3D pos = get_vec3D(obj.at("position"));
    const math::Vector3D rot = get_vec3D(obj.at("rotation"));
    const uint fov = static_cast<uint>(get_double(obj.at("fieldOfView")));

    return std::make_unique<raytracer::Camera>(vec2, pos, rot, fov);
}

/**
* @brief primitive factory entry point called by Core
* @details factory creating shared objects according to user parsed json configuration
* @return vector of shared Shape objects
*/
IShapesList primitive_factory(const ParsedJson &json_primitives)
{
    const auto &primitives = std::get<JsonMap>(json_primitives.value);
    const auto &spheres = std::get<Shapes>(primitives.at("spheres").value);
    const auto &rectangles = std::get<Shapes>(primitives.at("rectangles").value);

    /* reserve spheres size */
    std::vector<std::shared_ptr<raytracer::shape::IShape>> shapes(spheres.size() + rectangles.size());

    /* place spheres */
    for (const auto &e : spheres) {
        shapes.emplace_back(create_spheres(e));
    }
    for (const auto &e : rectangles) {
        shapes.emplace_back(create_rectangle(e));
    }
    return shapes;
}
