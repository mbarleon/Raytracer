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
        ALight(const math::RGBColor &color, double intensity);
        ~ALight() override = default;

        [[nodiscard]] math::RGBColor getColor() const override;
        [[nodiscard]] double getIntensity() const override;

    protected:
        math::RGBColor _color;
        double _intensity;
};
}
