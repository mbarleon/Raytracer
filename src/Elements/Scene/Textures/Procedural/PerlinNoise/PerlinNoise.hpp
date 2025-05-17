/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PerlinNoise
*/

#pragma once

#include "../../ITexture.hpp"
#include "Perlin/Perlin.hpp"

namespace raytracer::texture {
class PerlinNoise final : public ITexture {
    public:
        PerlinNoise(const double scale = 1.0);

        math::RGBColor value(const math::Point3D &p, const double u, const double v)
            const noexcept override;

    private:
        Perlin _perlin;
        double _scale;
};
}
