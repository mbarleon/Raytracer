/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.cpp
*/

#include "LimCylinder.hpp"
#include "Macro.hpp"
#include "Logger.hpp"
#include <cmath>

/*
 * public
 */
raytracer::shape::Cylinder::Cylinder(const math::Point3D &base_center, const math::Vector3D &axis, double radius, double height)
    : _base_center(base_center), _axis(axis.normalize()), _radius(radius), _height(height)
{
    logger::debug("Cylinder was built: base at ", base_center, " with axis ", axis, ", radius ", radius, " and height ", height, ".");
}

/**
 * @brief Return the base center of the cylinder.
 */
math::Vector3D raytracer::shape::Cylinder::getPosition() const
{
    return _base_center;
}

/**
 * @brief Compute the normal of the cylinder at a given point.
 */
math::Vector3D raytracer::shape::Cylinder::getNormalAt(const math::Point3D &point) const noexcept
{
    // Vector from base center to point
    const math::Vector3D base_to_point = point - _base_center;
    
    // Projection of base_to_point onto the axis
    const double projection = base_to_point.dot(_axis);
    
    // If point is on the base cap
    if (std::fabs(projection) < EPSILON) {
        return -_axis;
    }
    
    // If point is on the top cap
    if (std::fabs(projection - _height) < EPSILON) {
        return _axis;
    }
    
    // For points on the side, compute the normal by removing the axial component
    const math::Vector3D axial_component = _axis * projection;
    const math::Vector3D radial_component = base_to_point - axial_component;
    
    return radial_component.normalize();
}

/**
 * @brief Intersect the ray with the cylinder.
 */
bool raytracer::shape::Cylinder::intersect(const math::Ray &ray, math::Point3D &intPoint, const bool cullBackFaces) const noexcept
{
    const math::Vector3D oc = ray._origin - _base_center;
    const double axis_dir = ray._dir.dot(_axis);
    const double axis_oc = oc.dot(_axis);
    
    // Check for infinite cylinder intersection first
    const math::Vector3D dir_perp = ray._dir - _axis * axis_dir;
    const math::Vector3D oc_perp = oc - _axis * axis_oc;
    
    const double a = dir_perp.dot(dir_perp);
    const double b = 2.0 * dir_perp.dot(oc_perp);
    const double c = oc_perp.dot(oc_perp) - _radius * _radius;
    
    const double discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0) {
        return false;
    }
    
    const double sqrt_discriminant = std::sqrt(discriminant);
    const double t1 = (-b - sqrt_discriminant) / (2.0 * a);
    const double t2 = (-b + sqrt_discriminant) / (2.0 * a);
    
    double t = (t1 > 0.0) ? t1 : t2;
    if (t < 0.0) {
        return false;
    }
    
    // Check height constraints
    math::Point3D point = ray._origin + ray._dir * t;
    const double height = (point - _base_center).dot(_axis);
    
    if (height < 0.0 || height > _height) {
        // Check caps if infinite cylinder intersection failed
        bool hit_cap = false;
        double closest_t = std::numeric_limits<double>::max();
        
        // Check base cap
        const double denom_base = ray._dir.dot(-_axis);
        if (std::fabs(denom_base) > EPSILON) {
            const double t_base = (_base_center - ray._origin).dot(-_axis) / denom_base;
            if (t_base > 0.0 && t_base < closest_t) {
                const math::Point3D base_point = ray._origin + ray._dir * t_base;
                if ((base_point - _base_center).length() <= _radius) {
                    closest_t = t_base;
                    hit_cap = true;
                }
            }
        }
        
        // Check top cap
        const math::Point3D top_center = _base_center + _axis * _height;
        const double denom_top = ray._dir.dot(_axis);
        if (std::fabs(denom_top) > EPSILON) {
            const double t_top = (top_center - ray._origin).dot(_axis) / denom_top;
            if (t_top > 0.0 && t_top < closest_t) {
                const math::Point3D top_point = ray._origin + ray._dir * t_top;
                if ((top_point - top_center).length() <= _radius) {
                    closest_t = t_top;
                    hit_cap = true;
                }
            }
        }
        
        if (!hit_cap) {
            return false;
        }
        
        t = closest_t;
        point = ray._origin + ray._dir * t;
    }
    
    if (cullBackFaces) {
        const math::Vector3D normal = getNormalAt(point);
        if (ray._dir.dot(normal) > 0.0) {
            return false;
        }
    }
    
    intPoint = point;
    return true;
}