/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Diffuse
*/

#include "Diffuse.hpp"
#include <cmath>

math::RGBColor raytracer::material::DiffuseBSDF::sample(const math::Vector3D &wo,
    const math::Vector3D &normal, math::Vector3D &wi, double &pdf) const
{
    math::Vector3D tangent;
    math::Vector3D bitangent;
    buildOrthonormalBasis(normal, tangent, bitangent);

    const double r1 = getRandomDouble();
    const double r2 = getRandomDouble();
    const double theta = std::acos(std::sqrt(1.0f - r1));
    const double phi = 2.0f * M_PI * r2;

    const double x = std::sin(theta) * std::cos(phi);
    const double y = std::sin(theta) * std::sin(phi);
    const double z = std::cos(theta);

    wi = x * tangent + y * bitangent + z * normal;
    wi = wi.normalize();

    pdf = std::max(0.0, normal.dot(wi)) / M_PI;
    return albedo / M_PI;
}
