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

raytracer::shape::Rectangle::Rectangle(const math::Point3D &origin, const math::Vector3D &bottom_side,
    const math::Vector3D &left_side)
    : _origin(origin), _bottom_side(bottom_side), _left_side(left_side)
{
    logger::debug("Rectangle was built: origin ", origin, " bottom_side ", bottom_side, " left_side ", left_side, ".");
}

double raytracer::shape::Rectangle::getAOMaxDistance() const
{
    return 2.0; // To correct
}

math::RGBColor raytracer::shape::Rectangle::getColorAt(const math::Point3D __attribute__((unused)) &p) const
{
    return math::RGBColor(1);
}

/**
 * @brief
 * @details
 * @return
 */
bool raytracer::shape::Rectangle::intersect(const math::Ray &ray, math::Point3D __attribute__((unused)) &intPoint,
    __attribute__((unused)) const bool cullBackFaces) const noexcept
{
    const math::Vector3D normal = _bottom_side.cross(_left_side).normalize();
    const double denom = normal.dot(ray._dir);

    if (std::abs(denom) < 1e-6) {
        return false;
    }

    const double t = normal.dot(_origin - ray._origin) / denom;

    if (t < 0) {
        return false;
    }

    const math::Point3D hit = ray._origin + t * ray._dir;
    const math::Vector3D v = hit - _origin;

    const double bottom_length = _bottom_side.length();
    const double left_length = _left_side.length();

    const double proj_bottom = v.dot(_bottom_side) / bottom_length;
    const double proj_left = v.dot(_left_side / left_length);
    return proj_bottom >= 0 && proj_bottom <= bottom_length && proj_left >= 0 && proj_left <= left_length;
}

// TODO
math::Vector3D raytracer::shape::Rectangle::getPosition() const
{
    return math::Vector3D();
}

// TODO
math::Vector3D raytracer::shape::Rectangle::getNormalAt(const math::Point3D __attribute__((unused)) & point) const noexcept
{
    return math::Vector3D();
}
