/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point
*/

#pragma once

#include "../ALight.hpp"

namespace raytracer::light {
class Point final : public ALight
{
    public:
        Point(const math::RGBColor &color, const math::Point3D &position, const double intensity);

        material::BSDFSample sample(const math::Point3D &targetPoint) const override;

    private:
        math::Point3D _position;
};
}// namespace raytracer::light
