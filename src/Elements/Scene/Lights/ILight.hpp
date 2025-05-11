/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ILight
*/

#pragma once

#include "../../../Maths/Ray.hpp"

namespace raytracer::light {
class ILight {
    public:
        virtual ~ILight() = 0;

        [[nodiscard]] virtual math::RGBColor getColor() const = 0;
        [[nodiscard]] virtual math::Point3D getPosition() const = 0;
};
};
