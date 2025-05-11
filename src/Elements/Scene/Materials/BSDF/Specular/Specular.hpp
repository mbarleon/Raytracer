/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Specular
*/

#pragma once

#include "../BSDF.hpp"

namespace raytracer::material {
class SpecularBSDF : public BSDF {
    public:
        SpecularBSDF() = default;
        ~SpecularBSDF() override = default;

        BSDFSample sample(const math::Vector3D &wo,
            const math::Intersect &isect) const override;
        
        math::RGBColor evaluate(const math::Vector3D &wo, const math::Vector3D &wi,
            const math::Intersect &isect) const override;
};
};
