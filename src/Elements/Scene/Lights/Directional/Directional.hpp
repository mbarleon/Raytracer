/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Directional
*/

#pragma once

#include "../ALight.hpp"

namespace raytracer::light {
class Directional : public ALight {
    public:
        Directional(const math::RGBColor &color, const math::Vector3D &direction,
            const double intensity);

        material::BSDFSample sample(const math::Point3D &targetPoint) const override;

    private:
        math::Vector3D _direction;
};
}
