/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** MengerSponge
*/

#include "MengerSponge.hpp"

raytracer::shape::MengerSponge::MengerSponge(const math::Point3D &center, const double scale,
    const unsigned it, const unsigned maxSteps, const double maxDistance) noexcept :
    _center(center), _scale(scale), _it(it), _maxSteps(maxSteps), _maxDistance(maxDistance)
{
}

math::Vector3D raytracer::shape::MengerSponge::getNormalAt(const math::Point3D &point) const noexcept
{
    const math::Vector3D p = (point - _center) / _scale;

    return estimateNormal(p, _it);
}

void raytracer::shape::MengerSponge::getUV(const math::Point3D __attribute__((unused)) &p, double &u, double &v) const noexcept
{
    u = 0.5;
    v = 0.5;
}

math::RGBColor raytracer::shape::MengerSponge::getColorAt(const math::Point3D &p) const
{
    const math::Vector3D n = getNormalAt(p);
    const double intensity = std::fabs(n._y);

    return math::RGBColor(intensity, intensity, intensity);
}

math::Vector3D raytracer::shape::MengerSponge::getPosition() const
{
    return _center;
}

double raytracer::shape::MengerSponge::getAOMaxDistance() const
{
    return _scale * 2.0;
}

bool raytracer::shape::MengerSponge::intersect(const math::Ray &ray, math::Point3D &intPoint,
    const bool cullBackFaces) const noexcept
{
    const math::Vector3D origin = (ray._origin - _center) / _scale;
    const math::Vector3D direction = ray._dir.normalize();
    double t = 0.0;

    for (unsigned i = 0; i < _maxSteps && t < _maxDistance; ++i) {
        const math::Vector3D p = origin + direction * t;
        const double dist = distanceEstimator(p, _it);

        if (dist < 1e-3) {
            intPoint = ray._origin + ray._dir * t;

            if (cullBackFaces && direction.dot(getNormalAt(intPoint)) >= 0.0) {
                return false;
            }
            return true;
        }
        t += dist;
    }
    return false;
}

double raytracer::shape::MengerSponge::distanceEstimator(math::Vector3D p, const unsigned it) noexcept
{
    double scale = 1.0;

    for (unsigned i = 0; i < it; ++i) {
        p = math::Vector3D(
            std::fmod(p._x + 1.0, 2.0) - 1.0,
            std::fmod(p._y + 1.0, 2.0) - 1.0,
            std::fmod(p._z + 1.0, 2.0) - 1.0
        );

        if (p._x < 0.0) {
            p._x = -p._x;
        }
        if (p._y < 0.0) {
            p._y = -p._y;
        }
        if (p._z < 0.0) {
            p._z = -p._z;
        }
        if (p._x < p._y) {
            std::swap(p._x, p._y);
        }
        if (p._x < p._z) {
            std::swap(p._x, p._z);
        }

        p._x = p._x * 3.0 - 2.0 * (3.0 - 1.0);
        scale *= 3.0;
    }
    return (p.length() - 1.0) / scale;
}

math::Vector3D raytracer::shape::MengerSponge::estimateNormal(const math::Point3D &p,
    const unsigned it) noexcept
{
    const double eps = 1e-4 * std::max(1.0, 1e-4);
    const double d = distanceEstimator(p, it);

    math::Vector3D dx(p._x + eps, p._y, p._z);
    math::Vector3D dy(p._x, p._y + eps, p._z);
    math::Vector3D dz(p._x, p._y, p._z + eps);

    return math::Vector3D (distanceEstimator(dx, it) - d,
        distanceEstimator(dy, it) - d,
        distanceEstimator(dz, it) - d).normalize();
}
