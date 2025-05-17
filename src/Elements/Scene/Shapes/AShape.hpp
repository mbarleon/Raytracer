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
        void setShininess(const double shininess) override;
        void setTexture(const std::shared_ptr<texture::ITexture> &texture) override;

        material::Material getMaterial() const override;
        double getShininess() const override;

    protected:
        std::shared_ptr<raytracer::texture::ITexture> _texture;
        material::Material _material;
        double _shininess;
};
}// namespace raytracer::shape
