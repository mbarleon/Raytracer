/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rectangle.cpp
*/

#include "Rectangle.hpp"
#include "Logger.hpp"

/*
* public
*/

raytracer::shape::Rectangle::Rectangle(const math::Point3D &center, const double x,
    const double y, const double z) noexcept : _center(center), _x(x), _y(y), _z(z)
{
}

math::Vector3D raytracer::shape::Rectangle::getNormalAt(const math::Point3D &point)
    const noexcept
{
    const math::Vector3D p = point - _center;
    const double ax = std::fabs(p._x - _x);
    const double bx = std::fabs(p._x + _x);
    const double ay = std::fabs(p._y - _y);
    const double by = std::fabs(p._y + _y);
    const double az = std::fabs(p._z - _z);
    const double bz = std::fabs(p._z + _z);

    double m = ax;
    m = std::min(m, bx);
    m = std::min(m, ay);
    m = std::min(m, by);
    m = std::min(m, az);
    m = std::min(m, bz);

    if (m == ax)
        return math::Vector3D(1, 0, 0);
    if (m == bx)
        return math::Vector3D(-1, 0, 0);
    if (m == ay)
        return math::Vector3D(0, 1, 0);
    if (m == by)
        return math::Vector3D(0, -1, 0);
    if (m == az)
        return math::Vector3D(0, 0, 1);
    return math::Vector3D(0, 0, -1);
}

void raytracer::shape::Rectangle::getUV(const math::Point3D &p, double &u, double &v)
    const noexcept
{
    const math::Vector3D loc = p - _center;
    const math::Vector3D N = getNormalAt(p);

    if (std::fabs(N._x) > 0.5) {
        u = (loc._z / (_z) + 1.0) * 0.5;
        v = (loc._y / (_y) + 1.0) * 0.5;
    } else if (std::fabs(N._y) > 0.5) {
        u = (loc._x / (_x) + 1.0) * 0.5;
        v = (loc._z / (_z) + 1.0) * 0.5;
    } else {
        u = (loc._x / (_x) + 1.0) * 0.5;
        v = (loc._y / (_y) + 1.0) * 0.5;
    }
}

math::RGBColor raytracer::shape::Rectangle::getColorAt(const math::Point3D &p) const
{
    double u;
    double v;
    getUV(p, u, v);

    return _texture->value(p, u, v);
}

math::Vector3D raytracer::shape::Rectangle::getPosition() const
{
    return _center;
}

double raytracer::shape::Rectangle::getAOMaxDistance() const
{
    double maxDim = _x;
    maxDim = std::max(maxDim, _y);
    maxDim = std::max(maxDim, _z);

    return maxDim;
}

bool raytracer::shape::Rectangle::intersect(const math::Ray &ray, math::Point3D &intPoint,
    const bool cullBackFaces) const noexcept
{
    const math::Vector3D orig = ray._origin - _center;
    const math::Vector3D dir = ray._dir;

    double tmin = (-_x - orig._x) / dir._x;
    double tmax = (_x - orig._x) / dir._x;
    if (tmin > tmax)
        std::swap(tmin, tmax);

    double tymin = (-_y - orig._y) / dir._y;
    double tymax = (_y - orig._y) / dir._y;
    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    double tzmin = (-_z - orig._z) / dir._z;
    double tzmax = (_z - orig._z) / dir._z;
    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    const double t = (tmin >= 0.0 ? tmin : (tmax >= 0.0 ? tmax : -1.0));
    if (t < 0.0)
        return false;
    intPoint = ray._origin + ray._dir * t;

    if (const math::Vector3D N = getMappedNormal(intPoint); cullBackFaces && ray._dir.dot(N) >= 0.0)
        return false;
    return true;
}
