/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point
*/

#pragma once

#include "../ALight.hpp"
#include "Logger.hpp"

namespace raytracer::light {
class Point final : public ALight {
    public:
        Point(const math::RGBColor &color, const math::Point3D &point);
};
};
