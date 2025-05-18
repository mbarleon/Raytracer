/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** MengerSponge
*/

#pragma once

#include "../AShape.hpp"
#include <limits>

namespace raytracer::shape {
class MengerSponge final : public AShape {
    public:
        MengerSponge(const math::Point3D &center, const double scale, const unsigned it,
            const unsigned maxSteps, const double maxDistance) noexcept;

        math::Vector3D getNormalAt(const math::Point3D &point) const noexcept override;
        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
        math::RGBColor getColorAt(const math::Point3D &p) const override;
        math::Vector3D getPosition() const override;
        double getAOMaxDistance() const override;

        bool intersect(const math::Ray &ray, math::Point3D &intPoint,
            const bool cullBackFaces) const noexcept override;

    private:
        math::Point3D _center;
        double _scale;
        unsigned _it;
        unsigned _maxSteps;
        double _maxDistance;

        static double distanceEstimator(math::Vector3D p, const unsigned it) noexcept;
        static math::Vector3D estimateNormal(const math::Point3D &p, const unsigned it) noexcept;
};
} // namespace raytracer::shape
