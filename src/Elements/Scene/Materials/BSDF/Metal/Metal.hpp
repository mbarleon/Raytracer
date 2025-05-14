/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Metal
*/

#pragma once

#include "../BSDF.hpp"
#include "../../../../../Maths/Intersect.hpp"
#include <random>

namespace raytracer::material {
class MetalBSDF : public BSDF {
public:
    MetalBSDF(const math::RGBColor &albedo, double roughness);
    ~MetalBSDF() override = default;

    BSDFSample sample(const math::Vector3D &wo, const math::Intersect &isect,
        std::mt19937 &rng) const override;

    math::RGBColor evaluate(const math::Vector3D &wo, const math::Vector3D &wi,
        const math::Intersect &isect, std::mt19937 &rng) const override;

private:
    math::RGBColor _specular; // specular color
    double _roughnessSquare;
};
} // namespace raytracer::material
