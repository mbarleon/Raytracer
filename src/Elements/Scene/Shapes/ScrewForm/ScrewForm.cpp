/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ScrewForm
*/

#include "ScrewForm.hpp"
#include "Macro.hpp"

raytracer::shape::ScrewForm::ScrewForm(const math::Point3D &center, const double width,
    const double twistFreq, const unsigned maxSteps, const double maxDist) noexcept :
    _center(center), _halfWidth(width), _twistFreq(twistFreq), _maxSteps(maxSteps),
    _maxDist(maxDist)
{
}

math::Vector3D raytracer::shape::ScrewForm::getNormalAt(const math::Point3D &point)
    const noexcept
{
    const math::Vector3D base = point - _center;

    const double d0 = distanceEstimator(base);
    const double dx = distanceEstimator(base + math::Vector3D(EPSILON, 0, 0)) - d0;
    const double dy = distanceEstimator(base + math::Vector3D(0, EPSILON, 0)) - d0;
    const double dz = distanceEstimator(base + math::Vector3D(0, 0, EPSILON)) - d0;
    return math::Vector3D(dx,dy,dz).normalize();
}

void raytracer::shape::ScrewForm::getUV(const math::Point3D &p, double &u, double &v)
    const noexcept
{
    const math::Vector3D N = getNormalAt(p);
    const math::Vector3D loc = p - _center;

    if (std::fabs(N._x) > 0.5) {
        u = (loc._z/_halfWidth + 1.0)*0.5;
        v = (loc._y/_halfWidth + 1.0)*0.5;
    } else if (std::fabs(N._y) > 0.5) {
        u = (loc._x/_halfWidth + 1.0)*0.5;
        v = (loc._z/_halfWidth + 1.0)*0.5;
    } else {
        u = (loc._x/_halfWidth + 1.0)*0.5;
        v = (loc._y/_halfWidth + 1.0)*0.5;
    }
}

math::RGBColor raytracer::shape::ScrewForm::getColorAt(const math::Point3D &p) const
{
    double u;
    double v;

    getUV(p,u,v);
    return _texture->value(p,u,v);
}

math::Vector3D raytracer::shape::ScrewForm::getPosition() const
{
    return _center;
}

double raytracer::shape::ScrewForm::getAOMaxDistance() const
{
    return _halfWidth * 2.0;
}

bool raytracer::shape::ScrewForm::intersect(const math::Ray &ray, math::Point3D &intPoint,
    const bool cullBackFaces) const noexcept
{
    const math::Vector3D origin = (ray._origin - _center);
    const math::Vector3D direction = ray._dir.normalize();
    double t = 0.0;

    for (unsigned i = 0; i < _maxSteps && t < _maxDist; ++i) {
        const math::Vector3D p = origin + direction * t;
        const double d = distanceEstimator(p);

        if (d < EPSILON) {
            intPoint = ray._origin + ray._dir * t;

            if (const math::Vector3D N = getMappedNormal(intPoint); cullBackFaces && ray._dir.dot(N) >= 0.0)
                return false;
            return true;
        }
        t += d;
    }
    return false;
}

double raytracer::shape::ScrewForm::distanceEstimator(math::Vector3D p) const noexcept
{
    const double angle = _twistFreq * p._y;
    const double c = std::cos(angle);
    const double s = std::sin(angle);
    const double x = p._x * c - p._z * s;
    const double z = p._x * s + p._z * c;

    p._x = x;
    p._z = z;

    const math::Vector3D d = math::Vector3D(
        std::fabs(p._x) - _halfWidth,
        std::fabs(p._y) - _halfWidth,
        std::fabs(p._z) - _halfWidth
    );
    const double outside = math::Vector3D(std::max(d._x,0.0),
        std::max(d._y,0.0), std::max(d._z,0.0)).length();
    const double inside = std::min(std::max(d._x, std::max(d._y, d._z)), 0.0);
    return outside + inside;
}

math::Vector3D raytracer::shape::ScrewForm::estimateNormal(const math::Point3D &pt)
    const noexcept
{
    const math::Vector3D p = pt - _center;

    const double d0 = distanceEstimator(p);
    const double dx = distanceEstimator(p + math::Vector3D(EPSILON, 0.0, 0.0)) - d0;
    const double dy = distanceEstimator(p + math::Vector3D(0.0, EPSILON, 0.0)) - d0;
    const double dz = distanceEstimator(p + math::Vector3D(0.0, 0.0, EPSILON)) - d0;
    return math::Vector3D(dx, dy, dz).normalize();
}
