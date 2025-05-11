/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ALight
*/

#pragma once

#include "ILight.hpp"

namespace raytracer::light {
class ALight : public ILight {
    public:
        ALight(const math::RGBColor &color, const math::Point3D &point);
        ~ALight() override = default;

        [[nodiscard]] math::RGBColor getColor() const override;
        [[nodiscard]] math::Point3D getPosition() const override;

    protected:
        math::RGBColor _color;
        math::Point3D _position;
};
};
