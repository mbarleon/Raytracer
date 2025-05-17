/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ColorTexture
*/

#pragma once

#include "../ITexture.hpp"

namespace raytracer::texture {
class ColorTexture final : public ITexture {
    public:
        ColorTexture(const math::RGBColor &color) noexcept;

        math::RGBColor value(const math::Point3D &p, const double u,
            const double v) const noexcept override;

    private:
        math::RGBColor _color;
};
}
