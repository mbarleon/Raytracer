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

/**
* @brief
* @details
* @return
*/
raytracer::shape::Sphere::Sphere(const math::Point3D &center, const double radius) : _center(center), _radius(radius)
{
    logger::debug("sphere was built: ", center);
}

/**
* @brief
* @details
* @return
*/
bool raytracer::shape::Sphere::intersect(const math::Ray &ray, double &t) const noexcept
{
    const math::Vector3D o_c = ray._origin - _center;

    const double a = ray._dir.dot(ray._dir);
    const double b = 2.0 * o_c.dot(ray._dir);
    const double c = o_c.dot(o_c) - _radius * _radius;
    const double discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0)
        return false;

    const double sq = std::sqrt(discriminant);
    const double t0 = (-b - sq) / (2.0 * a);
    const double t1 = (-b + sq) / (2.0 * a);

    if (t0 > 0.0) {
        t = t0;
        return true;
    }
    if (t1 > 0.0) {
        t = t1;
        return true;
    }
    return false;
}
