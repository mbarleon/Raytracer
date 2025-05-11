/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BSDF
*/

#pragma once

#include "../../../Maths/Vector3D.hpp"
#include "Utils/Utils.hpp"

namespace raytracer::material {
class BSDF {
    public:
        virtual ~BSDF() = default;

        // wo : direction sortante (vers la caméra)
        // normal : normale au point d’intersection
        // wi : direction incidente échantillonnée (à retourner par référence)
        // pdf : densité de proba associée à ce choix de direction
        virtual math::RGBColor sample(const math::Vector3D &wo, const math::Vector3D &normal,
            math::Vector3D &wi, double &pdf) const = 0;
};
};
