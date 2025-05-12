/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Specular
*/

#include "Specular.hpp"
#include "../../../../../Maths/Intersect.hpp"

raytracer::material::BSDFSample raytracer::material::SpecularBSDF::sample(const math::Vector3D &wo,
    const math::Intersect &isect, std::mt19937 __attribute__((unused)) &rng) const
{
    const math::Vector3D reflected = reflect(-wo, isect.normal).normalize();
    const math::RGBColor color = isect.object->getColor();

    return {reflected, 1.0, color, true };
}

math::RGBColor raytracer::material::SpecularBSDF::evaluate(const math::Vector3D __attribute__((unused)) &wo,
    const math::Vector3D __attribute__((unused)) &wi, const math::Intersect __attribute__((unused)) &isect,
    std::mt19937 __attribute__((unused)) &rng) const
{
    const math::Vector3D expected = reflect(-wo, isect.normal).normalize();

    if ((wi - expected).nearZero()) {
        const double cosTheta = fabs(isect.normal.dot(wi));

        return isect.object->getColor() * (1.0 / cosTheta);
    }
    return math::RGBColor(0.0);
}
