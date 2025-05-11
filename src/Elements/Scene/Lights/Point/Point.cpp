/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point
*/

#include "Point.hpp"

raytracer::light::Point::Point(const math::RGBColor &color, const math::Point3D &position,
    double intensity) : ALight(color, intensity), _position(position)
{
    logger::debug("Light point was built: ", position, ".");
}

raytracer::material::BSDFSample raytracer::light::Point::sample(const math::Point3D &targetPoint) const
{
    math::Vector3D dir = _position - targetPoint;
    const double dist = dir.length();
    dir = dir / dist;

    const math::RGBColor radiance = (_color * _intensity) / (dist * dist);
    return { dir, dist, radiance };
}
