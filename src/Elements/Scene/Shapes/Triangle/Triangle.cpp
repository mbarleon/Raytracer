/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle.cpp
*/

#include "Triangle.hpp"
#include "Logger.hpp"
#include "Macro.hpp"
#include <algorithm>
#include <cmath>

/**
* public
*/

raytracer::shape::Triangle::Triangle(const math::Point3D &p0, const math::Point3D &p1, const math::Point3D &p2) noexcept
    : _p0(p0), _p1(p1), _p2(p2)
{
    init();
    logger::debug("Triangle was built: ", _p0, ", ", _p1, ", ", _p2, ".");
}

math::Vector3D raytracer::shape::Triangle::getPosition() const
{
    return math::Vector3D::applyRotation(_centroid, _rotation);
}

math::Vector3D raytracer::shape::Triangle::getNormalAt(const math::Point3D __attribute__((unused)) & p3d) const noexcept
{
    return math::Vector3D::applyRotation(_normal, _rotation).normalize();
}

double raytracer::shape::Triangle::getAOMaxDistance() const
{
    return _aomax;
}

/**
 * \explanation
 *
 * P = α * A + β * B + γ * C
 * UV_p = u_bary * UV0 + v_bary * UV1 + w_bary * UV2;
 *
 * where
 *   
 *   u_bary	Weight of vertex _p0
 *   v_bary	Weight of vertex _p1
 *   w_bary	Weight of vertex _p2
 *   u, v	Final UV mapping used for texture sampling
 */
void raytracer::shape::Triangle::getUV(const math::Point3D &p_world, double &u, double &v) const noexcept
{
    math::Vector3D p = math::Vector3D::applyRotation(p_world, -_rotation);

    const math::Vector3D v0 = _edge1;
    const math::Vector3D v1 = _edge2;
    const math::Vector3D v2 = p - _p0;

    const double d00 = v0.dot(v0);
    const double d01 = v0.dot(v1);
    const double d11 = v1.dot(v1);
    const double d20 = v2.dot(v0);
    const double d21 = v2.dot(v1);
    const double denom = d00 * d11 - d01 * d01;

    if (denom == 0.0) {
        u = v = 0.0;
        return;
    }

    const double v_bary = (d11 * d20 - d01 * d21) / denom;
    const double w_bary = (d00 * d21 - d01 * d20) / denom;
    // const double u_bary = 1.0 - v_bary - w_bary;

    u = v_bary;
    v = w_bary;
}

math::RGBColor raytracer::shape::Triangle::getColorAt(const math::Point3D &p) const
{
    double u;
    double v;

    getUV(p, u, v);
    if (_texture)
        return _texture->value(p, u, v);
    return math::RGBColor(0, 0, 0);
}

bool raytracer::shape::Triangle::intersect(const math::Ray &ray_world, math::Point3D &intPoint, const bool cullBackFaces) const noexcept
{
    math::Ray ray;
    ray._origin = math::Vector3D::applyRotation(ray_world._origin, -_rotation);
    ray._dir = math::Vector3D::applyRotation(ray_world._dir, -_rotation).normalize();

    const math::Vector3D h = ray._dir.cross(_edge2);
    const double a = _edge1.dot(h);

    if (std::abs(a) < EPSILON) {
        return false;
    }

    const double invA = 1.0 / a;
    const math::Vector3D s = ray._origin - _p0;
    const double u_bary = s.dot(h) * invA;

    if (u_bary < 0.0 || u_bary > 1.0) {
        return false;
    }

    const math::Vector3D q = s.cross(_edge1);
    const double v_bary = ray._dir.dot(q) * invA;

    if (v_bary < 0.0 || u_bary + v_bary > 1.0) {
        return false;
    }

    const double t = _edge2.dot(q) * invA;

    if (t < EPSILON) {
        return false;
    }

    math::Point3D p_local = ray._origin + ray._dir * t;
    intPoint = math::Vector3D::applyRotation(p_local, _rotation);

    if (cullBackFaces && ray_world._dir.dot(_normal) >= 0.0) {
        return false;
    }
    return true;
}

/**
* private
*/

void raytracer::shape::Triangle::init() noexcept
{
    _edge1 = _p1 - _p0;
    _edge2 = _p2 - _p0;
    _normal = _edge1.cross(_edge2).normalize();
    _centroid =
        math::Point3D((_p0._x + _p1._x + _p2._x) / 3.0, (_p0._y + _p1._y + _p2._y) / 3.0, (_p0._z + _p1._z + _p2._z) / 3.0);

    const double d0 = (_p0 - _centroid).length();
    const double d1 = (_p1 - _centroid).length();
    const double d2 = (_p2 - _centroid).length();

    _aomax = 2.0 * std::max({d0, d1, d2});
}
