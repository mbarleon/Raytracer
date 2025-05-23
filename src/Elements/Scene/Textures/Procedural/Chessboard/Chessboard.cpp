/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Chessboard
*/

#include "Chessboard.hpp"
#include <cmath>

raytracer::texture::Chessboard::Chessboard(const math::RGBColor &color1,
    const math::RGBColor &color2, const double scale) :
    _color1(color1), _color2(color2), _scale(scale)
{
}

math::RGBColor raytracer::texture::Chessboard::value(const math::Point3D &p,
    const double __attribute__((unused)) u, const double __attribute__((unused)) v) const noexcept
{
    const double s = std::sin(p._x / _scale) *
        std::sin(p._y / _scale) *
        std::sin(p._z / _scale);

    return (s < 0.0) ? _color1 : _color2;
}
