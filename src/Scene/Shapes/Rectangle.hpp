/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rectangle.hpp
*/

#pragma once

#include "AShape.hpp"

namespace raytracer::shape {
class Rectangle final : public AShape
{
    public:
        Rectangle(const math::Point3D &origin, const math::Vector3D &bottom_side, const math::Vector3D &left_side);

        [[nodiscard]] bool intersect(const math::Ray &ray, double &t) const noexcept override;

    private:
        math::Point3D _origin;
        math::Vector3D _bottom_side;
        math::Vector3D _left_side;
};
}// namespace raytracer::shape
