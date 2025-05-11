/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Specular
*/

#include "Specular.hpp"
#include "../../../../../Maths/Intersect.hpp"

raytracer::material::BSDFSample raytracer::material::SpecularBSDF::sample(const math::Vector3D &wo,
    const math::Intersect &isect) const
{
    const math::Vector3D reflected = reflect(-wo, isect.normal).normalize();
    const math::RGBColor color = isect.object->getColor();

    return {reflected, 1.0, color};
}
