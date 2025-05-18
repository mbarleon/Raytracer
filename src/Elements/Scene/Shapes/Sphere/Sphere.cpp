/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere.cpp
*/

#include "Sphere.hpp"
#include "Logger.hpp"
#include <cmath>

/*
* public
*/

raytracer::shape::Sphere::Sphere(const math::Point3D &center, const double radius) : _center(center), _radius(radius)
{
    logger::debug("Sphere was built: ", center, ".");
}

/**
* @brief
* @details
* @return
*/
math::Vector3D raytracer::shape::Sphere::getPosition() const
{
    return _center;
}

/**
* @brief
* @details
* @return
*/
math::Vector3D raytracer::shape::Sphere::getNormalAt(const math::Point3D &point) const noexcept
{
    const math::Vector3D normal = point - _center;

    return normal.normalize();
}

double raytracer::shape::Sphere::getAOMaxDistance() const
{
    return 2.0 * _radius;
}

void raytracer::shape::Sphere::getUV(const math::Point3D &p, double &u, double &v) const noexcept
{
    const math::Vector3D n = (p - _center).normalize();
    const double theta = std::acos(-n._y);
    const double phi = std::atan2(-n._z, n._x) + M_PI;

    u = phi / (2.0 * M_PI);
    v = theta / M_PI;
}

math::RGBColor raytracer::shape::Sphere::getColorAt(const math::Point3D &p) const
{
    double u;
    double v;

    getUV(p, u, v);
    return _texture->value(p, u, v);
}

/**
* @brief
* @details
* @return
*/
bool raytracer::shape::Sphere::intersect(const math::Ray &ray, math::Point3D &intPoint,
    __attribute__((unused)) const bool cullBackFaces) const noexcept
{
    const math::Vector3D o_c = ray._origin - _center;

    const double a = ray._dir.dot(ray._dir);
    const double b = 2.0 * o_c.dot(ray._dir);
    const double c = o_c.dot(o_c) - _radius * _radius;
    const double discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0)
        return false;

    const double sqrtDisc = std::sqrt(discriminant);
    const double t1 = (-b - sqrtDisc) / (2.0 * a);
    const double t2 = (-b + sqrtDisc) / (2.0 * a);
    const double t = (t1 >= 0.0 ? t1 : (t2 >= 0.0 ? t2 : -1.0));

    if (t < 0.0) {
        return false;
    }
    intPoint = ray._origin + ray._dir * t;

    if (const math::Vector3D N = getMappedNormal(intPoint); cullBackFaces && ray._dir.dot(N) >= 0.0) {
        return false;
    }
    return true;
}
