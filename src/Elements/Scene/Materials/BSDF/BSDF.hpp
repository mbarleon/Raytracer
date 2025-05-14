/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BSDF
*/

#pragma once

#include "../Utils/Utils.hpp"
#include "Sample.hpp"

namespace math {
struct Intersect;
}

namespace raytracer::material {
class BSDF
{
    public:
        virtual ~BSDF() = default;

        // wo : direction sortante (vers la caméra)
        // isect : infos d’intersection, incluant objet et normale
        virtual BSDFSample sample(const math::Vector3D &wo, const math::Intersect &isect) const = 0;

        virtual math::RGBColor evaluate(const math::Vector3D &wo, const math::Vector3D &wi,
            const math::Intersect &isect) const = 0;
};
};// namespace raytracer::material
