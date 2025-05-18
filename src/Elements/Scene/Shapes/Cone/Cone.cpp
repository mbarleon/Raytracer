/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.cpp
*/

#include "Cone.hpp"
#include "Logger.hpp"
#include "Macro.hpp"
#include <cmath>

/*
* public
*/

/**
 * @brief Cone __ctor__
 */
raytracer::shape::Cone::Cone(const math::Point3D &apex, const math::Vector3D &direction, const double angle, const double height)
    : _apex(apex), _direction(direction.normalize()), _angle(angle), _height(height)
{
    _cos_angle = std::cos(_angle);
    _sin_angle = std::sin(_angle);
    _tan_angle = std::tan(_angle);
    logger::debug("Cone was built: apex=", apex, ", angle=", angle, ", height=", height, ".");
}

/**
* @brief Cone::getPosition
* @details returns the position of the cone (its apex)
* @return cone apex position
*/
math::Vector3D raytracer::shape::Cone::getPosition() const
{
    return _apex;
}

/**
* @brief calculate UV coordinates for texture mapping on the cone
* @details maps the 3D point to 2D texture coordinates
*
*   \UV_mapping:
*       u = φ / 2π
*       v = 1 − h / H
*
*   \Where:
*       - φ is the azimuth angle around the cone (0 to 2π)
*       - h is the height from apex to point along the cone axis
*       - H is the total cone height
*
* @return void
*/
void raytracer::shape::Cone::getUV(const math::Point3D &p, double &u, double &v) const noexcept
{
    const math::Vector3D apex_to_p3d = p - _apex;
    const double height = apex_to_p3d.dot(_direction);
    const math::Vector3D project_on_plane = apex_to_p3d - _direction * height;
    const math::Vector3D normalized = project_on_plane.normalize();
    const double phi = std::atan2(normalized._z, normalized._x) + M_PI;

    u = phi / (2.0 * M_PI);
    v = 1.0 - height / _height;// <<< INFO: v=0 at base <|> v=1 at apex
}

/**
* @brief Cone::getNormalAt
* @details calculate the normal vector at a given point on the cone
* @return normalized vec3
*/
math::Vector3D raytracer::shape::Cone::getNormalAt(const math::Point3D &point) const noexcept
{
    const math::Vector3D apex_to_p3d = point - _apex;
    const double projection_len = apex_to_p3d.dot(_direction);
    const math::Vector3D projection_vec3 = _direction * projection_len;

    /**
     * @details normal point perpendicular to cone surface
     * vec3 from the point to the closest p3d on the axis, adjusted by cone angle
     */
    math::Vector3D normal = apex_to_p3d - projection_vec3;

    /**
     * @details adjust normal considering the cone angle
     *
     *   \equation:  𝐧 = 𝐧 × cos(𝛼) − 𝐯 × sin(𝛼)
     *
     *               this skews the vector to point perpendicular to the cone surface
     */
    normal = normal * _cos_angle - _direction * _sin_angle;

    return normal.normalize();
}

/**
* @brief Cone::getAOMaxDistance
* @details get the maximum distance for ambient occlusion calculations
* @return max dist for AO calculations (double)
*/
double raytracer::shape::Cone::getAOMaxDistance() const
{
    const double base = _height * _tan_angle;

    return std::sqrt(_height * _height + base * base);
}

/**
* @brief Cone::getColorAt
* @details get the color at UV coordinates
* @return math::RGBColor
*/
math::RGBColor raytracer::shape::Cone::getColorAt(const math::Point3D &p) const
{
    double u;
    double v;

    getUV(p, u, v);
    if (_texture) {
        return _texture->value(p, u, v);
    }
    return math::RGBColor(0, 0, 0);
}

/**
* @brief Cone::intersect
* @details computes the intersection using the quadratic formula, huge but trust me
*
* @formula:
*
*  \equation: quadratic equation: 𝑎𝑡² + 𝑏𝑡 + 𝑐 = 0
*
*       𝑎 = (𝐝·𝐯)² − cos²(𝛼)
*       𝑏 = 2 × [ (𝐝·𝐯)(𝐨·𝐯) − (𝐝·𝐨) × cos²(𝛼) ]
*       𝑐 = (𝐨·𝐯)² − |𝐨|² × cos²(𝛼)
*
*   \where:
*       - 𝐝 is the ray direction
*       - 𝐨 = ray origin − cone apex
*       - 𝐯 is the cone axis (normalized)
*       - 𝛼 is the cone angle (in radians)
*
* @return true <|> false
*/
bool raytracer::shape::Cone::intersect(const math::Ray &ray, math::Point3D &intPoint, const bool cullBackFaces) const noexcept
{
    const math::Vector3D origin_to_apex = ray._origin - _apex;
    const double dir_dot_cone_dir = ray._dir.dot(_direction);
    const double origin_dot_cone_dir = origin_to_apex.dot(_direction);
    const double cos_angleSq = _cos_angle * _cos_angle;

    /**
     * @details coefficients of the quadratic equation *> at² + bt + c = 0
     */
    const double a = dir_dot_cone_dir * dir_dot_cone_dir - cos_angleSq;
    const double b = 2.0 * (dir_dot_cone_dir * origin_dot_cone_dir - ray._dir.dot(origin_to_apex) * cos_angleSq);
    const double c = origin_dot_cone_dir * origin_dot_cone_dir - origin_to_apex.dot(origin_to_apex) * cos_angleSq;

    const double discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0) {
        return false;
    }
    if (std::abs(a) < EPSILON) {
        return false;
    }

    /**
     * @details find the closest intersection point
     */
    const double sqrt_discriminant = std::sqrt(discriminant);

    double t1 = (-b - sqrt_discriminant) / (2.0 * a);
    double t2 = (-b + sqrt_discriminant) / (2.0 * a);

    /**
     * @details sort intersection points
    */
    if (t1 > t2)
        std::swap(t1, t2);

    double t = t1;

    /**
    * @details check if the intersection point is within the cone height
    */
    const math::Point3D p1 = ray._origin + ray._dir * t1;
    const double h1 = (p1 - _apex).dot(_direction);

    if (h1 < 0 || h1 > _height) {
        t = t2;
        const math::Point3D p2 = ray._origin + ray._dir * t2;
        const double h2 = (p2 - _apex).dot(_direction);

        if (h2 < 0 || h2 > _height) {
            return false;
        }
    }

    if (t < 0.0) {
        return false;
    }

    intPoint = ray._origin + ray._dir * t;

    if (cullBackFaces) {
        const math::Vector3D N = getNormalAt(intPoint);

        if (ray._dir.dot(N) >= 0.0) {
            return false;
        }
    }

    return true;
}
