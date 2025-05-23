/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rectangle.hpp
*/

#pragma once

#include "../AShape.hpp"

namespace raytracer::shape {
class Rectangle final : public AShape
{
    public:
        Rectangle(const math::Point3D &center, const double x, const double y,
            const double z) noexcept;

        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;
        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
        math::RGBColor getColorAt(const math::Point3D &p) const override;
        math::Vector3D getPosition() const override;
        double getAOMaxDistance() const override;

        bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            const bool cullBackFaces) const noexcept override;

    private:
        math::Point3D _center;
        double _x;
        double _y;
        double _z;
};
}// namespace raytracer::shape
