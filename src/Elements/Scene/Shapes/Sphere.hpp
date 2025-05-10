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

        math::Vector3D getPosition() const override;
        math::Point3D getRandomPointOnSurface(std::mt19937 &rng) const override;
        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;

        [[nodiscard]] bool intersect(const math::Ray &ray, math::Point3D &intPoint, bool cullBackFaces) const noexcept override;

    private:
        math::Point3D _center;
        double _radius;
};
};// namespace raytracer::shape
