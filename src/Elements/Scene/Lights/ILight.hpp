/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ILight
*/

#pragma once

#include "../../../Maths/Ray.hpp"
#include "../Materials/BSDF/Sample.hpp"

namespace raytracer::light {
class ILight
{
    public:
        virtual ~ILight() = default;

        [[nodiscard]] virtual math::RGBColor getColor() const = 0;
        [[nodiscard]] virtual double getIntensity() const = 0;

        virtual material::BSDFSample sample(const math::Point3D &targetPoint) const = 0;
};
}// namespace raytracer::light
