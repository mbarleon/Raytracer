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
struct Material {
    std::shared_ptr<BSDF> bsdf;

    explicit Material();
    explicit Material(const std::shared_ptr<BSDF> &bsdf);

    BSDFSample sample(const math::Vector3D &wo, const math::Intersect &isect,
        std::mt19937 &rng) const;
};
}// namespace raytracer
// clang-format on
