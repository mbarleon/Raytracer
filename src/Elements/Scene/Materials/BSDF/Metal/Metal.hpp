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
class MetalBSDF final : public BSDF {
    public:
        MetalBSDF(const double roughness);
        ~MetalBSDF() override = default;

        BSDFSample sample(const math::Vector3D &wo, const math::Intersect &isect,
            std::mt19937 &rng) const override;

        math::RGBColor evaluate(const math::Vector3D &wo, const math::Vector3D &wi,
            const math::Intersect &isect, std::mt19937 &rng) const override;

    private:
        double _roughness;
};
}// namespace raytracer::material
