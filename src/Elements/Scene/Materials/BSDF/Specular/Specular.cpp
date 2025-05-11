/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Specular
*/

#include "Specular.hpp"

math::RGBColor raytracer::material::SpecularBSDF::sample(const math::Vector3D &wo,
    const math::Vector3D &normal, math::Vector3D &wi, double &pdf) const
{
    wi = reflect(-wo, normal);
    pdf = 1.0f;

    return albedo / std::abs(normal.dot(wi));
}
