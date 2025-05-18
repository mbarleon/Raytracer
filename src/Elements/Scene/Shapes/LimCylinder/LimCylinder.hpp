/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LimCylinder
*/

#pragma once

#include "../AShape.hpp"

namespace raytracer::shape {
class LimCylinder final : public AShape
{
    public:
        LimCylinder(const math::Point3D &origin, const double radius, const double height);

        math::Vector3D getPosition() const override;
        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;

        [[nodiscard]] bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            bool cullBackFaces) const noexcept override;
        ~LimCylinder();

    private:
        math::Point3D _origin;
        double _radius;
        double _height;

        
    protected:
};
}
