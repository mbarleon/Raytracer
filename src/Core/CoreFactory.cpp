/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.cpp
*/

#include "CoreFactory.hpp"
#include "../Elements/Scene/Shapes/Rectangle.hpp"
#include "../Elements/Scene/Shapes/Sphere.hpp"
#include "Error.hpp"
#include "Macro.hpp"
#include "../../include/Logger.hpp"
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
unit_static std::string get_string(const ParsedJson &proto)
{
    if (std::holds_alternative<std::string>(proto.value)) {
        return std::get<std::string>(proto.value);
    }
    throw raytracer::exception::Error("Core", "Expected string");
}

/**
* @brief
* @details private static
* @return
*/
unit_static raytracer::RGBColor get_color(const ParsedJson &proto)
{
    const auto &obj = std::get<JsonMap>(proto.value);

    return raytracer::RGBColor(get_double(obj.at("r")),
        get_double(obj.at("g")), get_double(obj.at("b")));
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
unit_static math::Vector2u get_vec2u(const ParsedJson &proto)
{
    const auto &obj = std::get<JsonMap>(proto.value);
    const double x = get_double(obj.at("width"));
    const double y = get_double(obj.at("height"));

    return math::Vector2u{static_cast<uint>(x), static_cast<uint>(y)};
}

/**
* @brief
* @details private static
* @return
*/
unit_static std::shared_ptr<raytracer::Material> get_material(const ParsedJson &proto, const MaterialsList &materials)
{
    const std::string mat_str = get_string(proto);

    auto it = materials.find(mat_str);
    if (it == materials.end())
        throw raytracer::exception::Error("Core", "Undefined material");
    return it->second;
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
unit_static void create_material(const ParsedJson &proto, MaterialsList &materials)
{
    const auto &obj = std::get<JsonMap>(proto.value);

    const std::string name = get_string(obj.at("name"));
    const double reflectivity = get_double(obj.at("reflectivity"));
    const double transparency = get_double(obj.at("transparency"));
    const double refractiveIndex = get_double(obj.at("refractive-index"));
    const double shininess = get_double(obj.at("shininess"));

    std::shared_ptr<raytracer::Material> material =
        std::make_shared<raytracer::Material>(reflectivity, transparency, refractiveIndex, shininess);
    materials[name] = material;
    raytracer::logger::debug("Material '", name, "' was built.");
}

/**
* @brief
* @details private static
* @return
*/
unit_static std::shared_ptr<raytracer::shape::Sphere> create_sphere(const ParsedJson &proto, const MaterialsList &materials)
{
    const auto &obj = std::get<JsonMap>(proto.value);
    const math::Vector3D position = get_vec3D(obj.at("position"));
    const double radius = get_double(obj.at("radius"));
    const std::shared_ptr<raytracer::Material> material = get_material(obj.at("material"), materials);
    const raytracer::RGBColor color = get_color(obj.at("color"));

    std::shared_ptr<raytracer::shape::Sphere> sphere =
        std::make_shared<raytracer::shape::Sphere>(position, radius);
    sphere.get()->setMaterial(material);
    sphere.get()->setColor(color);
    return sphere;
}

/**
* @brief
* @details private static
* @return
*/
unit_static std::shared_ptr<raytracer::shape::Rectangle> create_rectangle(const ParsedJson &proto, const MaterialsList &materials)
{
    const auto &obj = std::get<JsonMap>(proto.value);
    const math::Vector3D origin = get_vec3D(obj.at("origin"));
    const math::Vector3D bottom_side = get_vec3D(obj.at("bottom_side"));
    const math::Vector3D left_side = get_vec3D(obj.at("left_side"));
    const std::shared_ptr<raytracer::Material> material = get_material(obj.at("material"), materials);
    const raytracer::RGBColor color = get_color(obj.at("color"));

    std::shared_ptr<raytracer::shape::Rectangle> rectangle =
        std::make_shared<raytracer::shape::Rectangle>(origin, bottom_side, left_side);
    rectangle.get()->setMaterial(material);
    rectangle.get()->setColor(color);
    return rectangle;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief Entry Point
* @details Entry point
* @return TODO
*/
std::unique_ptr<raytracer::Camera> create_camera(const ParsedJson &camera_json)
{
    const auto &obj = std::get<JsonMap>(camera_json.value);
    const math::Vector2u vec2 = get_vec2u(obj.at("resolution"));
    const math::Point3D pos = get_vec3D(obj.at("position"));
    const math::Vector3D rot = get_vec3D(obj.at("rotation"));
    const uint fov = static_cast<uint>(get_double(obj.at("fov")));

    return std::make_unique<raytracer::Camera>(vec2, pos, rot, fov);
}

/**
* @brief Entry Point
* @details Entry point
* @return TODO
*/
std::unique_ptr<raytracer::Render> create_render(const ParsedJson &render_json)
{
    const auto &obj = std::get<JsonMap>(render_json.value);

    const raytracer::RGBColor bgColor = get_color(obj.at("background-color"));

    const auto &anti_obj = std::get<JsonMap>(obj.at("antialiasing").value);
    const raytracer::Antialiasing anti = {get_string(anti_obj.at("type")),
        static_cast<unsigned int>(get_double(anti_obj.at("samples")))};

    const auto &out_obj = std::get<JsonMap>(obj.at("output").value);
    const raytracer::RenderOutput output = {get_string(out_obj.at("file")),
        get_string(out_obj.at("format"))};

    const double mdepth = static_cast<uint>(get_double(obj.at("max-depth")));
    return std::make_unique<raytracer::Render>(bgColor, anti, output, mdepth);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief primitive factory entry point called by Core
* @details factory creating shared objects according to user parsed json configuration
* @return vector of shared Shape objects
*/
MaterialsList material_factory(const ParsedJson &json_scene)
{
    const auto &scene = std::get<JsonMap>(json_scene.value);
    MaterialsList materials;

    auto material_it = scene.find("materials");
    if (material_it != scene.end() && std::holds_alternative<Shapes>(material_it->second.value)) {
        const auto &mats = std::get<Shapes>(material_it->second.value);
        materials.reserve(mats.size());
        for (const auto &e : mats)
            create_material(e, materials);
    }
    return materials;
}

/**
* @brief primitive factory entry point called by Core
* @details factory creating shared objects according to user parsed json configuration
* @return vector of shared Shape objects
*/
IShapesList primitive_factory(const ParsedJson &json_primitives, const MaterialsList &materials)
{
    const auto &primitives = std::get<JsonMap>(json_primitives.value);
    IShapesList shapes;

    auto spheres_it = primitives.find("spheres");
    if (spheres_it != primitives.end() && std::holds_alternative<Shapes>(spheres_it->second.value)) {
        const auto &spheres = std::get<Shapes>(spheres_it->second.value);
        shapes.reserve(shapes.size() + spheres.size());
        for (const auto &e : spheres)
            shapes.emplace_back(create_sphere(e, materials));
    }
    auto rectangles_it = primitives.find("rectangles");
    if (rectangles_it != primitives.end() && std::holds_alternative<Shapes>(rectangles_it->second.value)) {
        const auto &rectangles = std::get<Shapes>(rectangles_it->second.value);
        shapes.reserve(shapes.size() + rectangles.size());
        for (const auto &e : rectangles)
            shapes.emplace_back(create_rectangle(e, materials));
    }
    return shapes;
}
