/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IShape.hpp
*/

#pragma once

#include "../../../Maths/Ray.hpp"
#include "../Materials/Material.hpp"
#include "../Textures/ITexture.hpp"
#include <memory>

namespace raytracer::shape {
class IShape
{
    public:
        virtual ~IShape() = default;

        virtual void setMaterial(const material::Material &material) = 0;
        virtual void setShininess(const double shininess) = 0;
        virtual void setTexture(const std::shared_ptr<texture::ITexture> &texture) = 0;
        virtual void setNormalMap(const std::shared_ptr<texture::ITexture> &map, const double strength) = 0;
        virtual void setRotation(const math::Vector3D &rotation) = 0;

        virtual void getUV(const math::Point3D &p, double &u, double &v) const noexcept = 0;
        [[nodiscard]] virtual math::RGBColor getColorAt(const math::Point3D &p) const = 0;
        [[nodiscard]] virtual material::Material getMaterial() const = 0;
        [[nodiscard]] virtual double getShininess() const = 0;
        [[nodiscard]] virtual math::Vector3D getPosition() const = 0;
        [[nodiscard]] virtual math::Vector3D getNormalAt(const math::Point3D &point) const noexcept = 0;
        [[nodiscard]] virtual double getAOMaxDistance() const = 0;
        [[nodiscard]] virtual math::Vector3D getMappedNormal(const math::Point3D &point) const noexcept = 0;

        [[nodiscard]] virtual bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            const bool cullBackFaces) const noexcept = 0;
};
}// namespace raytracer::shape
