/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ImageTexture
*/

#include "ImageTexture.hpp"
#include "Error.hpp"
#include <cmath>

raytracer::texture::ImageTexture::ImageTexture(const std::string &filename, const double scale) : _scale(scale)
{
    if (!_image.loadFromFile(filename)) {
        throw raytracer::exception::Error("ImageTexture", "Cannot load " + filename);
    }

    const auto size = _image.getSize();
    _width = size.x;
    _height = size.y;
}

raytracer::texture::ImageTexture::~ImageTexture() = default;

math::RGBColor raytracer::texture::ImageTexture::value(const math::Point3D __attribute__((unused)) & p, const double u,
    const double v) const noexcept
{
    double uu = u * _scale;
    double vv = v * _scale;
    uu = uu - std::floor(uu);
    vv = vv - std::floor(vv);

    const unsigned x = std::min(static_cast<unsigned>(uu * _width), _width - 1);
    const unsigned y = std::min(static_cast<unsigned>((1.0 - vv) * _height), _height - 1);

    const sf::Color c = _image.getPixel({x, y});
    return math::RGBColor(c.r / 255.0, c.g / 255.0, c.b / 255.0);
}
