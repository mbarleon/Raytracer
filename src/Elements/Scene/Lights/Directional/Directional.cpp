/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Directional
*/

#include "Directional.hpp"
#include "Logger.hpp"

raytracer::light::Directional::Directional(const math::RGBColor &color,
    const math::Vector3D &direction, const double intensity)
    : ALight(color, intensity), _direction(direction.normalize())
{
    logger::debug("Directional light was built: ", direction, ".");
}

raytracer::material::BSDFSample raytracer::light::Directional::sample(const math::Point3D __attribute__((unused)) &targetPoint) const
{
    return { -_direction, 1.0, _color * _intensity, true };
}
