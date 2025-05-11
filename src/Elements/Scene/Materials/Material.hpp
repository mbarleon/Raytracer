/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#pragma once

#include "BSDF/BSDF.hpp"

// clang-format off
namespace raytracer::material {
typedef struct material {
    BSDF *bsdf;

    material(BSDF *bsdf);

    math::RGBColor sample(const math::Vector3D &wo, const math::Vector3D &normal,
        math::Vector3D &wi, double &pdf) const;
} Material;
};// namespace raytracer
// clang-format on
