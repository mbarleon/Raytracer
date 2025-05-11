/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Diffuse
*/

#pragma once

#include "../BSDF.hpp"

namespace raytracer::material {
class DiffuseBSDF : public BSDF {
    public:
        DiffuseBSDF() = default;
        ~DiffuseBSDF() override = default;

        BSDFSample sample(const math::Vector3D &wo,
            const math::Intersect &isect) const override;
};
};
