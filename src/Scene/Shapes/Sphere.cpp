/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere.cpp
*/

#include "Sphere.hpp"
#include "Logger.hpp"

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
bool raytracer::shape::Sphere::intersect(const math::Ray &ray) const noexcept
{
    const math::Vector3D o_c = ray._origin - _center;

    const double a = ray._dir.dot(ray._dir);
    const double b = 2.0 * o_c.dot(ray._dir);
    const double c = o_c.dot(o_c) - _radius * _radius;
    const double discriminant = b * b - 4 * a * c;

    return discriminant >= 0;
}
