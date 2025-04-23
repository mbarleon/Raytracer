/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray.hpp
*/

#pragma once

#include "Vector3D.hpp"
namespace math {
// clang-format off

/**
* @brief struct representing a Ray
* @details struct because we want to avoid new/delete in render loop
* prefer to pass it as reference like `const Ray &`
*/
struct Ray {
    math::Point3D _origin;
    math::Vector3D _dir;

    math::Point3D at(const double t) const noexcept;
};
// clang-format on
}// namespace math
