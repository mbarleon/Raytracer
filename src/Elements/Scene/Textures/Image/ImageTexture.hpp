/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ImageTexture
*/

#pragma once

#include "../ITexture.hpp"
#include <string>
#include <vector>

namespace raytracer::texture {
class ImageTexture final : public ITexture {
    public:
        ImageTexture(const std::string &filename);

        math::RGBColor value(const math::Point3D &p, const double u,
            const double v) const noexcept override;

    private:
        int _width;
        int _height;
        std::vector<unsigned char> _data;
};
}
