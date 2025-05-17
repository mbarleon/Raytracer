/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ITexture
*/

#pragma once

#include "../../../Maths/Vector3D.hpp"

namespace raytracer::texture {
class ITexture {
    public:
        virtual ~ITexture() = default;

        virtual math::RGBColor value(const math::Point3D &p, const double u, const double v)
            const noexcept = 0;
};
}
