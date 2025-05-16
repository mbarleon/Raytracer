/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Specular
*/

#pragma once

#include "../BSDF.hpp"

namespace raytracer::material {
class SpecularBSDF final : public BSDF {
    public:
        SpecularBSDF() = default;
        ~SpecularBSDF() override = default;

        BSDFSample sample(const math::Vector3D &wo,
            const math::Intersect &isect, std::mt19937 &rng) const override;
        
        math::RGBColor evaluate(const math::Vector3D &wo, const math::Vector3D &wi,
            const math::Intersect &isect, std::mt19937 &rng) const override;
};
}
