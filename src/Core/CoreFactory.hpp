/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.hpp
*/

#pragma once

#include "../Parser/ParserTypes.hpp"
#include "../Scene/Camera/Camera.hpp"
#include "../Scene/Render/Render.hpp"
#include "../Scene/Material/Material.hpp"
#include "../Scene/Shapes/IShape.hpp"
#include <memory>

using ParsedJson = raytracer::parser::JsonProto;
using JsonMap = std::unordered_map<std::string, ParsedJson>;
using Shapes = std::vector<ParsedJson>;
using IShapesList = std::vector<std::shared_ptr<raytracer::shape::IShape>>;
using MaterialsList = std::unordered_map<std::string, std::shared_ptr<raytracer::Material>>;

/**
* TODO: templates?
*/

IShapesList primitive_factory(const ParsedJson &json_primitives, const MaterialsList &materials);
MaterialsList material_factory(const ParsedJson &json_scene);
std::unique_ptr<raytracer::Camera> create_camera(const ParsedJson &camera_json);
std::unique_ptr<raytracer::Render> create_render(const ParsedJson &render_json);

#if defined(UNIT_TESTS)
    #include "../Scene/Shapes/Rectangle.hpp"
    #include "../Scene/Shapes/Sphere.hpp"
    #include "Macro.hpp"
unit_static std::string get_string(const ParsedJson &proto);
unit_static raytracer::RGB_color get_color(const ParsedJson &proto);
unit_static double get_double(const ParsedJson &proto);
unit_static math::Vector3D get_vec3D(const ParsedJson &proto);
unit_static std::shared_ptr<raytracer::Material> get_material(const ParsedJson &proto, const MaterialsList &materials);
unit_static std::shared_ptr<raytracer::shape::Sphere> create_sphere(const ParsedJson &proto, const MaterialsList &materials);
unit_static std::shared_ptr<raytracer::shape::Rectangle> create_rectangle(const ParsedJson &proto, const MaterialsList &materials);
unit_static void create_material(const ParsedJson &proto, MaterialsList &materials);
#endif
