/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Diffuse
*/

#include "Diffuse.hpp"
#include "../../../../../Maths/Intersect.hpp"
#include <cmath>

raytracer::material::BSDFSample raytracer::material::DiffuseBSDF::sample(const math::Vector3D __attribute__((unused)) &wo,
    const math::Intersect &isect, std::mt19937 &rng) const
{
    const math::Vector3D wi = cosineHemisphere(isect.normal, rng);
    const double pdf = std::max(0.0, wi.dot(isect.normal)) / M_PI;

    return {wi, pdf, isect.object->getColor() / M_PI};
}

math::RGBColor raytracer::material::DiffuseBSDF::evaluate(const math::Vector3D __attribute__((unused)) &wo,
    const math::Vector3D __attribute__((unused)) &wi, const math::Intersect &isect,
    std::mt19937 __attribute__((unused)) &rng) const
{
    // f = albedo / π
    const math::RGBColor albedo = isect.object->getColor();

    return albedo / M_PI;
}
