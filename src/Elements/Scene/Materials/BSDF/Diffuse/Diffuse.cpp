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
    const math::Intersect &isect) const
{
    const math::Vector3D wi = cosineHemisphere(isect.normal);
    const double pdf = std::max(0.0, wi.dot(isect.normal)) / M_PI;

    return {wi, pdf, isect.object->getColor() / M_PI};
}
