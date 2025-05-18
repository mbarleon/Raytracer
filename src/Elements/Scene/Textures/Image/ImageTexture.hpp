/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ImageTexture
*/

#pragma once

#include "../ITexture.hpp"
#include <string>
#include <SFML/Graphics.hpp>

namespace raytracer::texture {
class ImageTexture final : public ITexture {
    public:
        ImageTexture(const std::string &filename, const double scale);
        ~ImageTexture();

        math::RGBColor value(const math::Point3D &p, const double u,
            const double v) const noexcept override;

    private:
        unsigned _width;
        unsigned _height;
        double _scale;
        sf::Image _image;
};
}
