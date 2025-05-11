/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShape.hpp
*/

#pragma once

#include "IShape.hpp"

namespace raytracer::shape {
class AShape : public IShape
{
    public:
        ~AShape() override = default;

        void setMaterial(const material::Material &material) override;
        void setColor(math::RGBColor color) override;

        material::Material getMaterial() const override;
        math::RGBColor getColor() const override;

    protected:
        material::Material _material;
        math::RGBColor _color;
};
};// namespace raytracer::shape
