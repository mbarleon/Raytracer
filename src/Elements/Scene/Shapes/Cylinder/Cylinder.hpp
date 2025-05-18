/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.hpp
*/

#pragma once

#include "../AShape.hpp"

namespace raytracer::shape {
class Cylinder final : public AShape
{
    public:
        Cylinder(const math::Point3D &base_center, const math::Vector3D &axis, double radius, double height);

        math::Vector3D getPosition() const override;
        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;

        [[nodiscard]] bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            bool cullBackFaces) const noexcept override;
        double getAOMaxDistance() const override;
        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
        math::RGBColor getColorAt(const math::Point3D &p) const override;

    private:
        math::Point3D _base_center;
        math::Vector3D _axis;
        double _radius;
        double _height;
};
};