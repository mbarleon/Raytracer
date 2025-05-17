/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SkyboxCube
*/

#pragma once

#include "../../ITexture.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <string>

namespace raytracer::texture {
class SkyboxCube final : public ITexture {
    public:
        /**
         * @param filename image format :
         * 
         *      o X X o
         *      X X X X
         * 
         * 'X' is a face of the cube map.
         * 'o' an empty area.
         */
        explicit SkyboxCube(const std::string &filename);
        ~SkyboxCube() override = default;

        math::RGBColor value(const math::Point3D &p, const double u,
            const double v) const noexcept override;

    private:
        enum SkyboxCubeFace {
            PosX,
            NegX,
            PosY,
            NegY,
            PosZ,
            NegZ
        };

        struct SkyboxCubeCell {
            SkyboxCubeFace f;
            unsigned cx;
            unsigned cy;
        };

        std::array<sf::Image, 6> _faces;
        unsigned _faceW, _faceH;

        void extractFaces(const sf::Image &src);
};
} // namespace raytracer::texture
