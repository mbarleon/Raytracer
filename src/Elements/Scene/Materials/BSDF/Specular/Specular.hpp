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
        math::RGBColor albedo;

        SpecularBSDF(const math::RGBColor &albedo) : albedo(albedo) {}

        math::RGBColor sample(const math::Vector3D &wo, const math::Vector3D &normal,
            math::Vector3D &wi, double &pdf) const override;
};
};
