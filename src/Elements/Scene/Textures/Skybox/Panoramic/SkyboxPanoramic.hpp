/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SkyboxPanoramic
*/

#pragma once

#include "../../ITexture.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace raytracer::texture {
class SkyboxPanoramic final : public ITexture {
    public:
        explicit SkyboxPanoramic(const std::string &filename);
        ~SkyboxPanoramic() override = default;

        math::RGBColor value(const math::Point3D &p, const double u,
            const double v) const noexcept override;

    private:
        sf::Image _image;
        unsigned _width;
        unsigned _height;
};
}
