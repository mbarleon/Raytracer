/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere.hpp
*/

#pragma once

#include "../AShape.hpp"

namespace raytracer::shape {
class Sphere final : public AShape
{
    public:
        Sphere(const math::Point3D &center, const double radius);

        math::Vector3D getPosition() const override;
        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;
        math::RGBColor getColorAt(const math::Point3D &p) const override;
        double getAOMaxDistance() const override;

        [[nodiscard]] bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            const bool cullBackFaces) const noexcept override;

    private:
        math::Point3D _center;
        double _radius;
};
}// namespace raytracer::shape
