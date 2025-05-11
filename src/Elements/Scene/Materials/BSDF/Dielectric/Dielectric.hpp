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
        math::RGBColor absorptionCoeff;
    
        DielectricBSDF(double etaExt, double etaInt, const math::RGBColor &absorptionCoeff);

        math::RGBColor sample(const math::Vector3D &wo, const math::Vector3D &normal,
            math::Vector3D &wi, double &pdf) const override;
};
};
