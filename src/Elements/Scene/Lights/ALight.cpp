/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ALight
*/

#include "ALight.hpp"

raytracer::light::ALight::ALight(const math::RGBColor &color, double intensity) :
    _color(color), _intensity(intensity)
{
}

math::RGBColor raytracer::light::ALight::getColor() const
{
    return _color;
}

double raytracer::light::ALight::getIntensity() const
{
    return _intensity;
}
