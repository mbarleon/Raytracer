/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rectangle.cpp (oriented box with full 3-axis rotation)
*/

#include "Rectangle.hpp"
#include "Logger.hpp"
#include "Macro.hpp"
#include <algorithm>

raytracer::shape::Rectangle::Rectangle(const math::Point3D &center, const double hx, const double hy,
    const double hz) noexcept : _center(center), _x(hx), _y(hy), _z(hz)
{
    logger::debug("Rectangle built: center=", center, " half-extents= (", hx, ",", hy, ",", hz, ")");
}

math::Vector3D raytracer::shape::Rectangle::getPosition() const
{
    return _center;
}

math::Vector3D raytracer::shape::Rectangle::getNormalAt(const math::Point3D &p_world) const noexcept
{
    math::Vector3D local_p = p_world - _center;
    local_p = math::Vector3D::applyRotation(local_p, -_rotation);

    const double ax = std::fabs(std::fabs(local_p._x) - _x);
    const double ay = std::fabs(std::fabs(local_p._y) - _y);
    const double az = std::fabs(std::fabs(local_p._z) - _z);

    if (ax <= ay && ax <= az) {
        local_p._x > 0 ? local_p = math::Vector3D(1,0,0) : local_p = math::Vector3D(-1,0,0);
    } else if (ay <= ax && ay <= az) {
        local_p._y > 0 ? local_p = math::Vector3D(0,1,0) : local_p = math::Vector3D(0,-1,0);
    } else {
        local_p._z > 0 ? local_p = math::Vector3D(0,0,1) : local_p = math::Vector3D(0,0,-1);
    }
    return math::Vector3D::applyRotation(local_p, _rotation);
}

void raytracer::shape::Rectangle::getUV(const math::Point3D &p_world, double &u, double &v) const noexcept
{
    math::Vector3D local_p = p_world - _center;
    local_p = math::Vector3D::applyRotation(local_p, -_rotation);

    math::Vector3D Nl;
    double ax = std::fabs(std::fabs(local_p._x) - _x);
    double ay = std::fabs(std::fabs(local_p._y) - _y);
    double az = std::fabs(std::fabs(local_p._z) - _z);

    if (ax <= ay && ax <= az) {
        Nl = local_p._x > 0 ? math::Vector3D(1,0,0) : math::Vector3D(-1,0,0);
        u = ( local_p._z / _z + 1.0 ) * 0.5;
        v = ( local_p._y / _y + 1.0 ) * 0.5;
    } else if (ay <= ax && ay <= az) {
        Nl = local_p._y > 0 ? math::Vector3D(0,1,0) : math::Vector3D(0,-1,0);
        u = ( local_p._x / _x + 1.0 ) * 0.5;
        v = ( local_p._z / _z + 1.0 ) * 0.5;
    } else {
        Nl = local_p._z > 0 ? math::Vector3D(0,0,1) : math::Vector3D(0,0,-1);
        u = ( local_p._x / _x + 1.0 ) * 0.5;
        v = ( local_p._y / _y + 1.0 ) * 0.5;
    }
}

math::RGBColor raytracer::shape::Rectangle::getColorAt(const math::Point3D &p_world) const
{
    double u;
    double v;

    getUV(p_world, u, v);
    if (_texture)
        return _texture->value(p_world, u, v);
    return math::RGBColor(0.0, 0.0, 0.0);
}

double raytracer::shape::Rectangle::getAOMaxDistance() const
{
    return std::max({ _x, _y, _z });
}

bool raytracer::shape::Rectangle::intersect(const math::Ray &ray_world, math::Point3D &intPoint,
    const bool cullBackFaces) const noexcept
{
    math::Ray ray_loc;
    ray_loc._origin = ray_world._origin - _center;
    ray_loc._origin = math::Vector3D::applyRotation(ray_loc._origin, -_rotation);
    ray_loc._dir = math::Vector3D::applyRotation(ray_world._dir, -_rotation).normalize();

    double tmin = (-_x - ray_loc._origin._x) / ray_loc._dir._x;
    double tmax = (+_x - ray_loc._origin._x) / ray_loc._dir._x;
    if (tmin > tmax)
        std::swap(tmin, tmax);

    double tymin = (-_y - ray_loc._origin._y) / ray_loc._dir._y;
    double tymax = (+_y - ray_loc._origin._y) / ray_loc._dir._y;
    if (tymin > tymax)
        std::swap(tymin, tymax);
    if (tmin > tymax || tymin > tmax)
        return false;

    tmin = std::max(tmin, tymin);
    tmax = std::min(tmax, tymax);
    double tzmin = (-_z - ray_loc._origin._z) / ray_loc._dir._z;
    double tzmax = (+_z - ray_loc._origin._z) / ray_loc._dir._z;
    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);
    if (tmin > tzmax || tzmin > tmax)
        return false;

    tmin = std::max(tmin, tzmin);
    tmax = std::min(tmax, tzmax);

    const double t = (tmin >= 0.0 ? tmin : (tmax >= 0.0 ? tmax : -1.0));
    if (t < 0.0)
        return false;

    const math::Vector3D p_loc = ray_loc._origin + ray_loc._dir * t;
    intPoint = math::Vector3D::applyRotation(p_loc, _rotation) + _center;

    if (cullBackFaces) {
        math::Vector3D Ng;
        const double ax = std::fabs(std::fabs(p_loc._x) - _x);
        const double ay = std::fabs(std::fabs(p_loc._y) - _y);
        const double az = std::fabs(std::fabs(p_loc._z) - _z);

        if (ax <= ay && ax <= az)
            Ng = p_loc._x > 0 ? math::Vector3D(1,0,0) : math::Vector3D(-1,0,0);
        else if (ay <= ax && ay <= az)
            Ng = p_loc._y > 0 ? math::Vector3D(0,1,0) : math::Vector3D(0,-1,0);
        else
            Ng = p_loc._z > 0 ? math::Vector3D(0,0,1) : math::Vector3D(0,0,-1);
        Ng = math::Vector3D::applyRotation(Ng, _rotation);

        if (ray_world._dir.dot(Ng) >= 0.0)
            return false;
    }
    return true;
}
