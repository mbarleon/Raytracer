/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Dielectric
*/

#pragma once

#include "../BSDF.hpp"

namespace raytracer::material {
class DielectricBSDF : public BSDF {
    public:
        double etaExt;
        double etaInt;

        DielectricBSDF(double etaExt, double etaInt);

        BSDFSample sample(const math::Vector3D &wo,
            const math::Intersect &isect) const override;
};
};
