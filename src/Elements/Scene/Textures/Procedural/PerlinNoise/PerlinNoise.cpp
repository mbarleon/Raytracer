/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PerlinNoise
*/

#include "PerlinNoise.hpp"

raytracer::texture::PerlinNoise::PerlinNoise(const double scale) : _scale(scale)
{
}

math::RGBColor raytracer::texture::PerlinNoise::value(const math::Point3D &p,
    const double __attribute__((unused)) u, const double __attribute__((unused)) v) const noexcept
{
    const double n = 0.5 * (1.0 + _perlin.noise(p * _scale));

    return math::RGBColor(n);
}
