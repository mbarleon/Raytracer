/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include "Camera.hpp"
#include "Logger.hpp"

// clang-format off

raytracer::Camera::Camera(const math::Vector2u &resolution, const math::Point3D &position, const math::Vector3D &rotation, const uint field_of_view)
    : _resolution(resolution), _position(position), _rotation(rotation), _fov(field_of_view)
{
    logger::debug("camera was built: resolution", resolution, " position", position, " rotation", rotation, " fieldOfView: ", field_of_view);
}

// clang-format on
