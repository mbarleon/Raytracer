/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Perlin
*/

#pragma once

#include "../../../../../../Maths/Vector3D.hpp"
#include <array>

namespace raytracer::texture {

using u8 = unsigned short;

class Perlin
{
    public:
        Perlin();

        double noise(const math::Point3D &p) const noexcept;

    private:
        static double fade(const double t) noexcept;
        static double lerp(const double t, const double a, const double b) noexcept;
        static double grad(const int hash, const double x, const double y, const double z) noexcept;

        std::array<u8, 512> _perm;
};
}// namespace raytracer::texture
