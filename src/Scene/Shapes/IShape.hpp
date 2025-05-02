/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IShape.hpp
*/

#pragma once

#include "../../Maths/Ray.hpp"

namespace raytracer::shape {
class IShape
{
    public:
        virtual ~IShape() = default;

        [[nodiscard]] virtual bool intersect(const math::Ray &ray, double &t) const noexcept = 0;
};
}// namespace raytracer::shape
