/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LimCylinder
*/

#include "LimCylinder.hpp"
#include "Logger.hpp"

/*
* public
*/

raytracer::shape::LimCylinder::LimCylinder(const math::Point3D &origin, const double radius, const double height)
    : _origin(origin), _radius(radius), _height(height)
{
    logger::debug("LimCylinder was built: origin ", origin, " radius ", radius, " height ", height, ".");
}

/**
 * @brief
 * @details
 * @return
 */
bool raytracer::shape::LimCylinder::intersect(const math::Ray &ray, math::Point3D &intPoint,
    __attribute__((unused)) const bool cullBackFaces) const noexcept
{
    const math::Vector3D ray_dir = ray._dir;
    const math::Vector3D ray_origin = ray._origin - _origin;
    const double a = ray_dir._x * ray_dir._x + ray_dir._z * ray_dir._z;
    const double b = 2 * (ray_origin._x * ray_dir._x + ray_origin._z * ray_dir._z);
    const double c = ray_origin._x * ray_origin._x + ray_origin._z * ray_origin._z - _radius * _radius;
    const double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    }

    double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
    double t2 = (-b + std::sqrt(discriminant)) / (2 * a);

    if (t1 > t2) {
        std::swap(t1, t2);
    }

    if (t1 < 0) {
        t1 = t2; // Use t2 instead
        if (t1 < 0) {
            return false; // Both intersections are behind the ray origin
        }
    }

    intPoint = ray._origin + t1 * ray_dir;

    if (intPoint._y < _origin._y || intPoint._y > _origin._y + _height) {
        return false; // Intersection is outside the cylinder height
    }

    return true;
}

math::Vector3D raytracer::shape::LimCylinder::getPosition() const
{
    return _origin;
}

math::Vector3D raytracer::shape::LimCylinder::getNormalAt(const math::Point3D &point) const noexcept
{
    math::Vector3D normal = point - _origin;
    normal._y = 0; // Ignore the y component for the normal
    normal = normal.normalize();
    return normal;
}

raytracer::shape::LimCylinder::~LimCylinder()
{
    logger::debug("LimCylinder was destroyed.");
}