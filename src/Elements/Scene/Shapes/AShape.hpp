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
        void setNormalMap(const std::shared_ptr<texture::ITexture> &map, const double strength) override;

        material::Material getMaterial() const override;
        math::Vector3D getMappedNormal(const math::Point3D &point) const noexcept override;
        double getShininess() const override;

    protected:
        static int solveQuartic(const double A, const double B, const double C,
            const double D, const double E, double roots[4]) noexcept;

        std::shared_ptr<raytracer::texture::ITexture> _texture = nullptr;
        std::shared_ptr<raytracer::texture::ITexture> _normalMap = nullptr;
        material::Material _material;
        double _normalStrength;
        double _shininess;
};
}// namespace raytracer::shape
