/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.cpp
*/

#include "CoreFactory.hpp"
#include "Error.hpp"

/**
* @brief
* @details private static
* @return
*/
unit_static double get_double(const raytracer::parser::JsonProto &proto)
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
unit_static math::Vector3D get_vec3D(const raytracer::parser::JsonProto &proto)
{
    const auto &obj = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(proto.value);

    return math::Vector3D(get_double(obj.at("x")), get_double(obj.at("y")), get_double(obj.at("z")));
}

/**
* @brief
* @details private static
* @return
*/
unit_static std::shared_ptr<raytracer::shape::Sphere> create_spheres(const raytracer::parser::JsonProto &proto)
{
    const auto &obj = std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(proto.value);
    const math::Vector3D center = get_vec3D(obj.at("position"));
    const double radius = get_double(obj.at("radius"));

    return std::make_shared<raytracer::shape::Sphere>(center, radius);
}

/**
* @brief ENTRY POINT
* @details ENTRY POINT
* @return TODO
*/
std::vector<std::shared_ptr<raytracer::shape::Sphere>> get_spheres(const raytracer::parser::JsonProto &primitives)
{
    const auto &primitivesObj =
        std::get<std::unordered_map<std::string, raytracer::parser::JsonProto>>(primitives.value);
    const auto &spheresArray = std::get<std::vector<raytracer::parser::JsonProto>>(primitivesObj.at("spheres").value);
    std::vector<std::shared_ptr<raytracer::shape::Sphere>> spheres;

    for (const auto &sphereJson : spheresArray) {
        spheres.emplace_back(create_spheres(sphereJson));
    }
    return spheres;
}
