/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point
*/

#include "Point.hpp"

raytracer::light::Point::Point(const math::RGBColor &color, const math::Point3D &point) :
    ALight(color, point)
{
    logger::debug("Light point was built: ", point, ".");
}
