/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.hpp
*/

#pragma once

#include "../Parser/ParserTypes.hpp"
#include "../Shapes/IShape.hpp"
#include <memory>

using ParsedJson = raytracer::parser::JsonProto;
using Primitives = std::unordered_map<std::string, ParsedJson>;
using Shapes = std::vector<ParsedJson>;

std::vector<std::shared_ptr<raytracer::shape::IShape>> primitive_factory(
    const raytracer::parser::JsonProto &primitives);

#if defined(UNIT_TESTS)
    #include "../Shapes/Rectangle.hpp"
    #include "../Shapes/Sphere.hpp"
    #include "Macro.hpp"
unit_static double get_double(const raytracer::parser::JsonProto &proto);
unit_static math::Vector3D get_vec3D(const raytracer::parser::JsonProto &proto);
unit_static std::shared_ptr<raytracer::shape::Sphere> create_spheres(const raytracer::parser::JsonProto &proto);
unit_static std::shared_ptr<raytracer::shape::Rectangle> create_rectangle(const raytracer::parser::JsonProto &proto);
unit_static std::shared_ptr<raytracer::shape::Rectangle> create_rectangle(const raytracer::parser::JsonProto &proto);
#endif
