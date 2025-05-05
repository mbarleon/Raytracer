/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IShape.hpp
*/

#pragma once

#include "../Material/Material.hpp"
#include "../../../Maths/Ray.hpp"
#include "../Material/Color.hpp"
#include <memory>

namespace raytracer::shape {
class IShape
{
    public:
        virtual ~IShape() = default;

        virtual void setMaterial(const std::shared_ptr<Material> &material) = 0;
        virtual void setColor(RGBColor color) = 0;

        virtual std::shared_ptr<Material> getMaterial() const = 0;
        virtual RGBColor getColor() const = 0;

        [[nodiscard]] virtual bool intersect(const math::Ray &ray, math::Point3D &intPoint) const noexcept = 0;
};
}// namespace raytracer::shape
