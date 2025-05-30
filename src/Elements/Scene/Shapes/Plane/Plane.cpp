/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane
*/

#include "Plane.hpp"
#include "Logger.hpp"
#include "Macro.hpp"
#include <cmath>
#include <limits>

raytracer::shape::Plane::Plane(const char axis, const double position) noexcept : _axis(axis), _position(position)
{
    logger::debug("Plane built: axis=", axis, " position=", position);
}

math::Vector3D raytracer::shape::Plane::getPosition() const
{
    switch (_axis) {
        case 'X':
            return math::Vector3D(_position, 0.0, 0.0);
        case 'Y':
            return math::Vector3D(0.0, _position, 0.0);
        default:
            return math::Vector3D(0.0, 0.0, _position);
    }
}

math::Vector3D raytracer::shape::Plane::getNormalAt(const math::Point3D __attribute__((unused)) &point) const noexcept
{
    switch (_axis) {
        case 'X':
            return math::Vector3D(1.0, 0.0, 0.0);
        case 'Y':
            return math::Vector3D(0.0, 1.0, 0.0);
        default:
            return math::Vector3D(0.0, 0.0, 1.0);
    }
}

double raytracer::shape::Plane::getAOMaxDistance() const
{
    return std::numeric_limits<double>::infinity();
}

bool raytracer::shape::Plane::intersect(const math::Ray &ray, math::Point3D &intPoint, __attribute__((unused)) const bool cullBackFaces) const noexcept
{
    double t;

    switch (_axis) {
        case 'X':
            if (std::abs(ray._dir._x) < EPSILON) {
                return false;
            }
            t = (_position - ray._origin._x) / ray._dir._x;
            break;
        case 'Y':
            if (std::abs(ray._dir._y) < EPSILON) {
                return false;
            }
            t = (_position - ray._origin._y) / ray._dir._y;
            break;
        default:
            if (std::abs(ray._dir._z) < EPSILON) {
                return false;
            }
            t = (_position - ray._origin._z) / ray._dir._z;
            break;
    }
    if (t < 0.0) {
        return false;
    }
    intPoint = ray._origin + ray._dir * t;

    if (const math::Vector3D N = getMappedNormal(intPoint); cullBackFaces && ray._dir.dot(N) >= 0.0) {
        return false;
    }
    return true;
}

void raytracer::shape::Plane::getUV(const math::Point3D &p, double &u, double &v) const noexcept
{
    switch (_axis) {
        case 'X':
            u = p._z;
            v = p._y;
            break;
        case 'Y':
            u = p._x;
            v = p._z;
            break;
        default:
            u = p._x;
            v = p._y;
            break;
    }
}

math::RGBColor raytracer::shape::Plane::getColorAt(const math::Point3D &p) const
{
    double u;
    double v;

    getUV(p, u, v);
    return _texture->value(p, u, v);
}
