/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "../Elements/Render/Render.hpp"
#include "../Elements/Scene/Materials/Material.hpp"
#include "../Elements/Scene/Shapes/IShape.hpp"
#include "../Elements/Scene/Lights/ILight.hpp"
#include "../Parser/ParserTypes.hpp"
#include <memory>

using ParsedJson = raytracer::parser::JsonProto;
using JsonMap = std::unordered_map<std::string, ParsedJson>;
using Shapes = std::vector<ParsedJson>;
using ILightsList = std::vector<std::shared_ptr<raytracer::light::ILight>>;
using IShapesList = std::vector<std::shared_ptr<raytracer::shape::IShape>>;

/**
* TODO: templates?
*/

ILightsList light_factory(const ParsedJson &json_lights);
IShapesList primitive_factory(const ParsedJson &json_primitives);
std::unique_ptr<raytracer::Camera> create_camera(const ParsedJson &camera_json);
std::unique_ptr<raytracer::Render> create_render(const ParsedJson &render_json);

#if defined(UNIT_TESTS)
    #include "../Elements/Scene/Shapes/Rectangle/Rectangle.hpp"
    #include "../Elements/Scene/Shapes/Sphere/Sphere.hpp"
    #include "Macro.hpp"
unit_static double clamp_color(double component);
unit_static std::string get_string(const ParsedJson &proto);
unit_static math::RGBColor get_color(const ParsedJson &proto);
unit_static math::Vector3D get_vec3D(const ParsedJson &proto);
unit_static raytracer::material::Material get_material(const JsonMap &obj);
unit_static std::shared_ptr<raytracer::shape::Sphere> create_sphere(const ParsedJson &proto);
unit_static std::shared_ptr<raytracer::shape::Rectangle> create_rectangle(const ParsedJson &proto);
#endif
