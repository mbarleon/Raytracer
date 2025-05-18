/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SkyboxPanoramic
*/

#include "SkyboxPanoramic.hpp"
#include "Error.hpp"
#include <cmath>

raytracer::texture::SkyboxPanoramic::SkyboxPanoramic(const std::string &filename)
{
    if (!_image.loadFromFile(filename)) {
        throw raytracer::exception::Error("SkyboxPanoramic", "Cannot load " + filename);
    }

    const sf::Vector2u imageSize = _image.getSize();

    _width = imageSize.x;
    _height = imageSize.y;
}

math::RGBColor raytracer::texture::SkyboxPanoramic::value(const math::Point3D &p, const double __attribute__((unused)) u,
    const double __attribute__((unused)) v) const noexcept
{
    const math::Vector3D d = p.normalize();
    const double phi = std::atan2(d._x, d._z);
    const double theta = std::asin(std::clamp(d._y, -1.0, 1.0));

    double uu = 0.5 + (phi / (2.0 * M_PI));
    double vv = 0.5 - (theta / M_PI);

    uu = uu - std::floor(uu);
    vv = vv - std::floor(v);

    const unsigned x = std::min(static_cast<unsigned>(uu * _width), _width - 1);
    const unsigned y = std::min(static_cast<unsigned>(vv * _height), _height - 1);

    const sf::Color c = _image.getPixel({x, y});
    return math::RGBColor(c.r / 255.0, c.g / 255.0, c.b / 255.0);
}
