/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.hpp
*/

#pragma once

#include "../AShape.hpp"

namespace raytracer::shape {

// clang-format off
/**
* @class Cone
* @brief represents a cone shape in 3D
* @details the cone is defined by its apex, direction angle and height
*/
class Cone final : public AShape
{
    public:
        explicit Cone(const math::Point3D &apex, const math::Vector3D &direction, const double angle, const double height) noexcept;

        [[nodiscard]] double getAOMaxDistance() const override;
        [[nodiscard]] math::Vector3D getPosition() const override;
        [[nodiscard]] math::RGBColor getColorAt(const math::Point3D &p) const override;
        [[nodiscard]] math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;
        [[nodiscard]] bool intersect(const math::Ray &ray, math::Point3D &intPoint, const bool cullBackFaces) const noexcept override;

        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;

    private:
        math::Point3D _apex;
        math::Vector3D _direction;
        double _angle;
        double _height;
        double _cos_angle;
        double _sin_angle;
        double _tan_angle;
};
// clang-format on
}// namespace raytracer::shape
