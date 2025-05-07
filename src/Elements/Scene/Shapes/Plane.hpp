/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane
*/

#pragma once

#include "../../../Maths/Vector3D.hpp"
#include "AShape.hpp"

namespace raytracer::shape {
class Plane final : public AShape
{
    public:
        Plane(char axis, double position) noexcept;

        math::Vector3D getPosition() const override;
        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;
        bool intersect(const math::Ray &ray, math::Point3D &intPoint) const noexcept override;

    private:
        char _axis;
        double _position;
};
};// namespace raytracer::shape
