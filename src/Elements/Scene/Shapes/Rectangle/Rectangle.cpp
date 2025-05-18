/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rectangle.cpp
*/

#include "Rectangle.hpp"
#include "Macro.hpp"
#include "Logger.hpp"
#include <cmath>

/*
 * public
 */
raytracer::shape::Rectangle::Rectangle(const math::Point3D &origin, const math::Vector3D &bottom_side, const math::Vector3D &left_side, const math::Vector3D &depth_side)
    : _origin(origin), _bottom_side(bottom_side), _left_side(left_side), _depth_side(depth_side)
{
    logger::debug("Rectangle was built: ", origin, " with sides: ", bottom_side, ", ", left_side, ", ", depth_side, ".");
}

/**
 * @brief Retturn the origin of the shape.
 */
math::Vector3D raytracer::shape::Rectangle::getPosition() const
{
    return _origin;
}

/**
 * @brief Compute the normal of the rectangle.
 */
math::Vector3D raytracer::shape::Rectangle::getNormalAt(const math::Point3D &point) const noexcept
{
    const std::array<std::tuple<math::Point3D, math::Vector3D, math::Vector3D, math::Vector3D>, 6> faces = {
        std::make_tuple(_origin, _bottom_side, _left_side, _bottom_side.cross(_left_side)), // Front face
        std::make_tuple(_origin + _depth_side, _bottom_side, _left_side, _left_side.cross(_bottom_side)), // Back face
        std::make_tuple(_origin, _bottom_side, _depth_side, _depth_side.cross(_bottom_side)), // Bottom face
        std::make_tuple(_origin + _left_side, _bottom_side, _depth_side, _bottom_side.cross(_depth_side)), // Top face
        std::make_tuple(_origin, _left_side, _depth_side, _left_side.cross(_depth_side)), // Left face
        std::make_tuple(_origin + _bottom_side, _left_side, _depth_side, _depth_side.cross(_left_side)) // Right face
    };

    constexpr double FACE_EPSILON = 1e-5;
    
    math::Vector3D accumulatedNormal(0, 0, 0);
    int matchingFaces = 0;

    for (const auto &[faceOrigin, side1, side2, faceNormal] : faces) {
        const auto normalizedNormal = faceNormal.normalize();
        
        const double distanceToPlane = std::fabs((point - faceOrigin).dot(normalizedNormal));
        
        if (distanceToPlane > FACE_EPSILON)
            continue;
            
        const auto v0 = side1;
        const auto v1 = side2;
        const auto v2 = point - faceOrigin;

        const double d00 = v0.dot(v0);
        const double d01 = v0.dot(v1);
        const double d11 = v1.dot(v1);
        const double d20 = v2.dot(v0);
        const double d21 = v2.dot(v1);

        const double denom = d00 * d11 - d01 * d01;
        if (std::fabs(denom) < EPSILON)
            continue;

        const double u = (d11 * d20 - d01 * d21) / denom;
        const double v = (d00 * d21 - d01 * d20) / denom;

        if (u >= -EPSILON && u <= 1.0 + EPSILON && v >= -EPSILON && v <= 1.0 + EPSILON) {
            accumulatedNormal = accumulatedNormal + normalizedNormal;
            matchingFaces++;
        }
    }

    if (matchingFaces == 0) {
        logger::debug("getNormalAt failed to find face for point: ", point);
        return _bottom_side.cross(_left_side).normalize();
    }

    return accumulatedNormal.normalize();
}

/**
 * @brief Intersect the ray with the rectangle.
 */
bool raytracer::shape::Rectangle::intersect(const math::Ray &ray, math::Point3D &intPoint, const bool cullBackFaces) const noexcept
{
    const std::array<std::tuple<math::Point3D, math::Vector3D, math::Vector3D>, 6> faces = {
        std::make_tuple(_origin, _bottom_side, _left_side), // Front face
        std::make_tuple(_origin + _depth_side, _bottom_side, _left_side), // Back face
        std::make_tuple(_origin, _bottom_side, _depth_side), // Bottom face
        std::make_tuple(_origin + _left_side, _bottom_side, _depth_side), // Top face
        std::make_tuple(_origin, _left_side, _depth_side), // Left face
        std::make_tuple(_origin + _bottom_side, _left_side, _depth_side) // Right face
    };

    bool hit = false;
    double closestT = std::numeric_limits<double>::max();

    for (const auto &[faceOrigin, side1, side2] : faces) {
        const auto N = side1.cross(side2).normalize();
        const double denom = ray._dir.dot(N);

        if (std::fabs(denom) < EPSILON)
            continue;
        if (cullBackFaces && denom > 0.0)
            continue;

        const auto orig_to_plane = faceOrigin - ray._origin;
        const double t = orig_to_plane.dot(N) / denom;
        if (t < 0.0 || t > closestT)
            continue;

        const auto P = ray._origin + ray._dir * t;
        const auto v0 = side1;
        const auto v1 = side2;
        const auto v2 = P - faceOrigin;

        const double d00 = v0.dot(v0);
        const double d01 = v0.dot(v1);
        const double d11 = v1.dot(v1);
        const double d20 = v2.dot(v0);
        const double d21 = v2.dot(v1);

        const double denom2 = d00 * d11 - d01 * d01;
        if (std::fabs(denom2) < EPSILON)
            continue;

        const double u = (d11 * d20 - d01 * d21) / denom2;
        const double v = (d00 * d21 - d01 * d20) / denom2;

        if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0) {
            closestT = t;
            intPoint = P;
            hit = true;
        }
    }

    return hit;
}
