/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane
*/

#pragma once

#include "../AShape.hpp"

namespace raytracer::shape {
class Plane final : public AShape
{
    public:
        Plane(const char axis, const double position) noexcept;

        math::Vector3D getPosition() const override;
        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;
        math::RGBColor getColorAt(const math::Point3D &p) const override;
        double getAOMaxDistance() const override;

        bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            const bool cullBackFaces) const noexcept override;

    private:
        char _axis;
        double _position;
};
}// namespace raytracer::shape
