/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Specular
*/

#include "Specular.hpp"

raytracer::material::BSDFSample raytracer::material::SpecularBSDF::sample(const math::Vector3D &wo,
    const math::Intersect &isect) const
{
    math::Vector3D wi = reflect(-wo, isect.normal);
    math::RGBColor color = isect.object->getColor();

    return {wi, 1.0, color};
}
