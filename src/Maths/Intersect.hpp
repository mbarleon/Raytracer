/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Intersect
*/

#pragma once
#include "../Elements/Scene/Shapes/IShape.hpp"
#include "Vector3D.hpp"

// clang-format off
namespace math {
typedef struct Intersect {
    std::shared_ptr<raytracer::shape::IShape> object;
    Point3D point;
    Vector3D normal;
    Ray reflected;
    double distance;
} Intersect;
};// namespace math
// clang-format on
