/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** TangleCube
*/

#pragma once

#include "../AShape.hpp"

namespace raytracer::shape {
class TangleCube final : public AShape {
public:
    TangleCube(const math::Point3D &center, const double size, const double thickness);

    math::Vector3D getPosition() const override;
    void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
    math::Vector3D getNormalAt(const math::Point3D &p) const noexcept override;
    math::RGBColor getColorAt(const math::Point3D &p) const override;
    double getAOMaxDistance() const override;

    bool intersect(const math::Ray &ray, math::Point3D &intPoint,
        bool cullBackFaces) const noexcept override;

private:
    math::Point3D _center;
    double _s;
    double _t;
};
} // namespace
