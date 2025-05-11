/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#pragma once

#include "BSDF/BSDF.hpp"
#include <memory>

// clang-format off
namespace raytracer::material {
typedef struct material {
    std::shared_ptr<BSDF> bsdf;

    material();
    material(std::shared_ptr<BSDF> bsdf);

    BSDFSample sample(const math::Vector3D &wo, const math::Intersect &isect) const;
} Material;
};// namespace raytracer
// clang-format on
