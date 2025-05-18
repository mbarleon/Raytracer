/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** TangleCube
*/

#pragma once

#include "../AShape.hpp"
#include "../../../../Maths/Vector3D.hpp"
#include "../../../../Maths/Ray.hpp"

namespace raytracer::shape {

/**
 * @brief Cube creux (parois de `thickness`), bords arrondis (rayon `radius`),
 *        centré en _center, arête extérieure = width.
 */
class TangleCube final : public AShape {
public:
    TangleCube(const math::Point3D &center,
               double width,
               double thickness,
               double radius,
               unsigned maxSteps = 128,
               double   maxDist   = 100.0) noexcept;

    // IShape
    bool intersect(const math::Ray &ray,
                   math::Point3D &intPoint,
                   const bool cullBackFaces) const noexcept override;

    math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;
    void           getUV     (const math::Point3D &p, double &u, double &v) const noexcept override;
    math::RGBColor getColorAt(const math::Point3D &p) const override;
    math::Vector3D getPosition()       const override { return _center; }
    double         getAOMaxDistance() const override { return _halfWidth * 2.0; }

private:
    math::Point3D _center;
    double        _halfWidth;
    double        _thickness;
    double        _radius;
    int           _maxSteps;
    double        _maxDist;

    // SDF du cube creux arrondi
    double distanceEstimator(const math::Vector3D &p) const noexcept;
    // Gradient pour normale
    math::Vector3D estimateNormal(const math::Point3D &p) const noexcept;

    // SDF d’une box axis‑aligned de demi‑taille b
    static double boxSDF(const math::Vector3D &p, const math::Vector3D &b) noexcept {
        auto q = math::Vector3D(
            std::fabs(p._x) - b._x,
            std::fabs(p._y) - b._y,
            std::fabs(p._z) - b._z
        );
        double outside = math::Vector3D(
            std::max(q._x, 0.0),
            std::max(q._y, 0.0),
            std::max(q._z, 0.0)
        ).length();
        double inside = std::min(std::max(q._x, std::max(q._y, q._z)), 0.0);
        return outside + inside;
    }
};

} // namespace raytracer::shape
