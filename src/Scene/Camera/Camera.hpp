/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.hpp
*/

#pragma once

#include "../../Maths/Vector2u.hpp"
#include "../../Maths/Vector3D.hpp"
#include "Macro.hpp"

// clang-format off
namespace raytracer {

class Camera final
{
    public:
        explicit Camera(const math::Vector2u &resolution, const math::Point3D &position, const math::Vector3D &rotation, const uint field_of_view);
        ~Camera() = default;

    private:
        math::Vector2u _resolution;
        math::Point3D _position;
        math::Vector3D _rotation;
        uint _fov = DEFAULT_FIELD_OF_VIEW;
};
}// namespace raytracer
// clang-format on
