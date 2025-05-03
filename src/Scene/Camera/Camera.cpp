/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include "Camera.hpp"
#include "Logger.hpp"
#include <cmath>
#include <fstream>

// clang-format off

raytracer::Camera::Camera(const math::Vector2u &resolution, const math::Point3D &position,
    const math::Vector3D &rotation, const uint field_of_view) : _resolution(resolution),
    _position(position), _rotation(rotation), _fov(field_of_view)
{
    logger::debug("camera was built: resolution", resolution, " position", position,
        " rotation", rotation, " fov: ", field_of_view);
}

void raytracer::Camera::render(const std::vector<std::shared_ptr<shape::IShape>> &shapes) const noexcept
{
    std::ofstream ppm("output.ppm");
    if (!ppm.is_open()) {
        std::cerr << "Failed to open output.ppm for writing.\n";
        return;
    }

    ppm << "P3\n"
        << _resolution.x << " " << _resolution.y << "\n"
        << "255\n";

    math::Ray r = {_position, math::Vector3D()};

    for (unsigned y = 0; y < _resolution.y; ++y) {
        for (unsigned x = 0; x < _resolution.x; ++x) {
            double u = (x + 0.5) / double(_resolution.x);
            double v = (y + 0.5) / double(_resolution.y);
            ray(u, v, r);

            double t_min = std::numeric_limits<double>::infinity();
            bool hit = false;
            for (const auto &shape : shapes) {
                double t;
                if (shape->intersect(r, t) && t < t_min) {
                    t_min = t;
                    hit = true;
                }
            }

            if (hit) {
                ppm << "255 255 255 ";
            } else {
                ppm << "135 206 235 ";
            }
        }
        ppm << "\n";
    }
}

// static math::Vector3D applyRotation(const math::Vector3D &dir, const math::Vector3D &rot)
// {
//     const double pitch = rot._x, yaw = rot._y, roll = rot._z;
//     double cosX = std::cos(pitch), sinX = std::sin(pitch);
//     double cosY = std::cos(yaw),   sinY = std::sin(yaw);
//     double cosZ = std::cos(roll),  sinZ = std::sin(roll);

//     double m00 = cosY * cosZ;
//     double m01 = cosZ * sinX * sinY - sinZ * cosX;
//     double m02 = cosZ * cosX * sinY + sinZ * sinX;

//     double m10 = cosY * sinZ;
//     double m11 = sinZ * sinX * sinY + cosZ * cosX;
//     double m12 = sinZ * cosX * sinY - cosZ * sinX;

//     double m20 = -sinY;
//     double m21 = cosY * sinX;
//     double m22 = cosY * cosX;

//     return math::Vector3D(
//         m00 * dir._x + m01 * dir._y + m02 * dir._z,
//         m10 * dir._x + m11 * dir._y + m12 * dir._z,
//         m20 * dir._x + m21 * dir._y + m22 * dir._z
//     );
// }

void raytracer::Camera::ray(double u, double v, math::Ray &cameraRay) const noexcept
{
    double aspect_ratio = static_cast<double>(_resolution.x) / static_cast<double>(_resolution.y);
    double fov_adjustment = std::tan((_fov * M_PI / 180.0) / 2.0);

    cameraRay._dir._x = (2.0 * u - 1.0) * aspect_ratio * fov_adjustment;
    cameraRay._dir._y = (1.0 - 2.0 * v) * fov_adjustment;
    cameraRay._dir._z = -1.0;
    cameraRay._dir = cameraRay._dir.normalize();
    // cameraRay._dir = applyRotation(cameraRay._dir, _rotation).normalize();
}

// clang-format on
