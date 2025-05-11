/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ALight
*/

#include "ALight.hpp"

raytracer::light::ALight::ALight(const math::RGBColor &color, const math::Point3D &point) :
    _color(color), _position(point)
{
}

math::RGBColor raytracer::light::ALight::getColor() const
{
    return _color;
}

math::Point3D raytracer::light::ALight::getPosition() const
{
    return _position;
}
