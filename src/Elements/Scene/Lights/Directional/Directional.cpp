/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Directional
*/

#include "Directional.hpp"
#include "Logger.hpp"
#include <limits>

raytracer::light::Directional::Directional(const math::RGBColor &color,
    const math::Vector3D &direction, double intensity)
    : ALight(color, intensity), _direction(direction.normalize())
{
    logger::debug("Light directional was built: ", direction, ".");
}

raytracer::material::BSDFSample raytracer::light::Directional::sample(const math::Point3D __attribute__((unused)) &targetPoint) const
{
    return {-_direction, std::numeric_limits<double>::infinity(), _color * _intensity};
}
