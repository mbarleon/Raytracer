/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ScrewForm
*/

#pragma once

#include "../AShape.hpp"
#include "../../../../Maths/Vector3D.hpp"
#include "../../../../Maths/Ray.hpp"

namespace raytracer::shape {

class ScrewForm final : public AShape {
    public:
        ScrewForm(const math::Point3D &center, const double width,
            const double twistFreq = 3.0, const unsigned maxSteps = 128,
            const double maxDist = 100.0) noexcept;

        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;
        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
        math::RGBColor getColorAt(const math::Point3D &p) const override;
        math::Vector3D getPosition() const override;
        double getAOMaxDistance() const override;

        bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            const bool cullBackFaces) const noexcept override;

    private:
        math::Point3D _center;
        double _halfWidth;
        double _twistFreq;
        unsigned _maxSteps;
        double _maxDist;

        double distanceEstimator(math::Vector3D p) const noexcept;
        math::Vector3D estimateNormal(const math::Point3D &p) const noexcept;
};
} // namespace raytracer::shape
