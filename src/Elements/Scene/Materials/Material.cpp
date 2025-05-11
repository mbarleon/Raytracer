/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#include "Material.hpp"

raytracer::material::material::material(BSDF *bsdf) : bsdf(bsdf)
{
}

math::RGBColor raytracer::material::material::sample(const math::Vector3D &wo,
    const math::Vector3D &normal, math::Vector3D &wi, double &pdf) const
{
    return bsdf->sample(wo, normal, wi, pdf);
}
