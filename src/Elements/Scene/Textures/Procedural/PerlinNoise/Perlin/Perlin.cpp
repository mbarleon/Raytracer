/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Perlin
*/

#include "Perlin.hpp"
#include <algorithm>
#include <cmath>
#include <random>

raytracer::texture::Perlin::Perlin()
{
    std::iota(_perm.begin(), _perm.begin() + 256, 0);
    std::mt19937 gen{42};
    std::shuffle(_perm.begin(), _perm.begin() + 256, gen);

    for (u8 i = 0; i < 256; ++i) {
        _perm[256 + i] = _perm[i];
    }
}

double raytracer::texture::Perlin::fade(const double t) noexcept
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double raytracer::texture::Perlin::lerp(const double t, const double a, const double b) noexcept
{
    return a + t * (b - a);
}

double raytracer::texture::Perlin::grad(const int hash, const double x, const double y, const double z) noexcept
{
    const int h = hash & 15;
    const double u = (h < 8) ? x : y;
    const double v = (h < 4) ? y : (h == 12 || h == 14 ? x : z);

    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

double raytracer::texture::Perlin::noise(const math::Point3D &p) const noexcept
{
    const u8 X = static_cast<u8>(std::floor(p._x)) & 255;
    const u8 Y = static_cast<u8>(std::floor(p._y)) & 255;
    const u8 Z = static_cast<u8>(std::floor(p._z)) & 255;

    const double x = p._x - std::floor(p._x);
    const double y = p._y - std::floor(p._y);
    const double z = p._z - std::floor(p._z);

    const double u = fade(x);
    const double v = fade(y);
    const double w = fade(z);

    const u8 A = _perm[X] + Y;
    const u8 AA = _perm[A] + Z;
    const u8 AB = _perm[A + 1] + Z;
    const u8 B = _perm[X + 1] + Y;
    const u8 BA = _perm[B] + Z;
    const u8 BB = _perm[B + 1] + Z;

    return lerp(w,
        lerp(v, lerp(u, grad(_perm[AA], x, y, z), grad(_perm[BA], x - 1, y, z)),
            lerp(u, grad(_perm[AB], x, y - 1, z), grad(_perm[BB], x - 1, y - 1, z))),
        lerp(v, lerp(u, grad(_perm[AA + 1], x, y, z - 1), grad(_perm[BA + 1], x - 1, y, z - 1)),
            lerp(u, grad(_perm[AB + 1], x, y - 1, z - 1), grad(_perm[BB + 1], x - 1, y - 1, z - 1))));
}
