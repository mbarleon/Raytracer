/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Dielectric
*/

#pragma once

#include "../BSDF.hpp"

namespace raytracer::material {
class DielectricBSDF final : public BSDF
{
    public:
        double _etaExt;
        double _etaInt;

        DielectricBSDF(const double etaExt, const double etaInt);
        ~DielectricBSDF() override = default;

        BSDFSample sample(const math::Vector3D &wo, const math::Intersect &isect, std::mt19937 &rng) const override;

        math::RGBColor evaluate(const math::Vector3D &wo, const math::Vector3D &wi, const math::Intersect &isect,
            std::mt19937 &rng) const override;
};
}// namespace raytracer::material
