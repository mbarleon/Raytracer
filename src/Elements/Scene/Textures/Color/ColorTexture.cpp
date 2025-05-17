/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ColorTexture
*/

#include "ColorTexture.hpp"

raytracer::texture::ColorTexture::ColorTexture(const math::RGBColor &color) noexcept :
    _color(color)
{
}

math::RGBColor raytracer::texture::ColorTexture::value(const math::Point3D &p,
    const double u, const double v) const noexcept
{
    return _color;
}
