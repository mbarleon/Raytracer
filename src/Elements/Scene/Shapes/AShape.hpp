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

        void setMaterial(const std::shared_ptr<Material> &material) override;
        void setColor(math::RGBColor color) override;

        std::shared_ptr<Material> getMaterial() const override;
        math::RGBColor getColor() const override;

    protected:
        std::shared_ptr<Material> _material;
        math::RGBColor _color;
};
};// namespace raytracer::shape
