/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BSDF
*/

#pragma once

#include "Sample.hpp"
#include "../../../../Maths/Intersect.hpp"
#include "Utils/Utils.hpp"

namespace raytracer::material {
class BSDF {
    public:
        virtual ~BSDF() = default;

        // wo : direction sortante (vers la caméra)
        // isect : infos d’intersection, incluant objet et normale
        virtual BSDFSample sample(const math::Vector3D &wo,
            const math::Intersect &isect) const = 0;
};
};
