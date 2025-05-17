/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Chessboard
*/

#pragma once

#include "../../ITexture.hpp"
#include <memory>

namespace raytracer::texture {
class Chessboard final : public ITexture {
    public:
        Chessboard(const math::RGBColor &color1, const math::RGBColor &color2,
            const double scale = 1.0);

        math::RGBColor value(const math::Point3D &p, const double u, const double v)
            const noexcept override;

    private:
        math::RGBColor _color1;
        math::RGBColor _color2;
        double _scale;
};
}
