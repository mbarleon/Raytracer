/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PerlinNoise
*/

#include "PerlinNoise.hpp"

raytracer::texture::PerlinNoise::PerlinNoise(const math::RGBColor &color1,
    const math::RGBColor &color2, const double scale) : _color1(color1),
    _color2(color2), _scale(scale)
{
}

math::RGBColor raytracer::texture::PerlinNoise::value(const math::Point3D &p,
    const double __attribute__((unused)) u, const double __attribute__((unused)) v) const noexcept
{
    const double n = _perlin.noise(p * _scale);
    const double t = 0.5 * (1.0 + n);

    return _color1 * (1.0 - t) + _color2 * t;
}
