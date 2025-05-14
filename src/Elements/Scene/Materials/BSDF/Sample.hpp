/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sample
*/

#pragma once

#include "../../../../Maths/Vector3D.hpp"

// clang-format off
namespace raytracer::material {
struct BSDFSample {
    math::Vector3D direction;
    double pdf;
    math::RGBColor radiance;
};
};// namespace raytracer::material
// clang-format on
