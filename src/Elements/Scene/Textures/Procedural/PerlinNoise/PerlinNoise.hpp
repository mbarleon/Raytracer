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
        PerlinNoise(const math::RGBColor &color1, const math::RGBColor &color2,
            const double scale);

        math::RGBColor value(const math::Point3D &p, const double u, const double v)
            const noexcept override;

    private:
        Perlin _perlin;
        math::RGBColor _color1;
        math::RGBColor _color2;
        double _scale;
};
}
