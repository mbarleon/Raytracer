/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreFactory.hpp
*/

#pragma once

#include "../Parser/ParserTypes.hpp"
#include "../Shapes/Sphere.hpp"
#include "Macro.hpp"
#include <memory>

std::vector<std::shared_ptr<raytracer::shape::IShape>> primitive_factory(
    const raytracer::parser::JsonProto &primitives);

#if defined(UNIT_TESTS)
unit_static double get_double(const raytracer::parser::JsonProto &proto);
unit_static math::Vector3D get_vec3D(const raytracer::parser::JsonProto &proto);
unit_static std::shared_ptr<raytracer::shape::Sphere> create_spheres(const raytracer::parser::JsonProto &proto);
#endif
