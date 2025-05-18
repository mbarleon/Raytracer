/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus
*/

#pragma once

#include "../AShape.hpp"

namespace raytracer::shape {
class Torus final : public AShape {
    public:
        Torus(const math::Point3D &center, const double majorRadius,
            const double minorRadius);

        math::Vector3D getPosition() const override;
        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
        math::Vector3D getNormalAt(const math::Point3D &p) const noexcept override;
        math::RGBColor getColorAt(const math::Point3D &p) const override;
        double getAOMaxDistance() const override;

        bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            const bool cullBackFaces) const noexcept override;

    private:
        math::Point3D _center;
        double _R;
        double _r;
};
} // namespace
