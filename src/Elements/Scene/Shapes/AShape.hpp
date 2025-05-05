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
        AShape() = default;
        ~AShape() override = default;

        void setMaterial(const std::shared_ptr<Material> &material) override;
        void setColor(RGBColor color) override;

        std::shared_ptr<Material> getMaterial() const override;
        RGBColor getColor() const override;

    protected:
        std::shared_ptr<Material> _material;
        RGBColor _color;
};
}// namespace raytracer::shape
