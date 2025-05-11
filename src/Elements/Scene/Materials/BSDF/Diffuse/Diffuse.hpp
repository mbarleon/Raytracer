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
        math::RGBColor albedo;
    
        DiffuseBSDF(const math::RGBColor &albedo) : albedo(albedo) {}

        math::RGBColor sample(const math::Vector3D &wo, const math::Vector3D &normal,
            math::Vector3D &wi, double &pdf) const override;
};
};
