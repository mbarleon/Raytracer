/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere.hpp
*/

#pragma once

#include "AShape.hpp"

namespace raytracer::shape {
class Sphere final : public AShape
{
    public:
        Sphere(const math::Point3D &center, double radius);

        [[nodiscard]] bool intersect(const math::Ray &ray, double &t) const noexcept override;

    private:
        math::Point3D _center;
        double _radius;
};
}// namespace raytracer::shape
