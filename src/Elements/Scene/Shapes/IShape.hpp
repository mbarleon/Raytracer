/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IShape.hpp
*/

#pragma once

#include "../Materials/Material.hpp"
#include "../../../Maths/Ray.hpp"
#include <memory>
#include <random>

namespace raytracer::shape {
class IShape
{
    public:
        virtual ~IShape() = default;

        virtual void setMaterial(const material::Material &material) = 0;
        virtual void setColor(math::RGBColor color) = 0;

        [[nodiscard]] virtual material::Material getMaterial() const = 0;
        [[nodiscard]] virtual math::RGBColor getColor() const = 0;
        [[nodiscard]] virtual math::Vector3D getPosition() const = 0;
        [[nodiscard]] virtual math::Vector3D getNormalAt(const math::Point3D &point) const noexcept = 0;

        [[nodiscard]] virtual bool intersect(const math::Ray &ray, math::Point3D &intPoint, bool cullBackFaces) const noexcept = 0;
};
};// namespace raytracer::shape
