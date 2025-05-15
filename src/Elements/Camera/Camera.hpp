/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.hpp
*/

#pragma once

#include "../../Maths/Vector2u.hpp"
#include "Logic/Pathtracer.hpp"

// clang-format off
namespace raytracer {

    class Camera final
    {
        public:
            explicit Camera(const math::Vector2u &resolution, const math::Point3D &position,
                const math::Vector3D &rotation, const unsigned int fov);
            ~Camera() = default;

            void generateRay(const double u, const double v, math::Ray &cameraRay) const noexcept;

            [[nodiscard]] uint getFov() const noexcept;
            [[nodiscard]] const math::Point3D getPosition() const noexcept;
            [[nodiscard]] const math::Vector3D getRotation() const noexcept;
            [[nodiscard]] const math::Vector2u getResolution() const noexcept;
            [[nodiscard]] const raytracer::RaytraceGrid2D render(const IShapesList &shapes, const ILightsList &lights, const RenderConfig &render) const;

        private:
            math::Vector2u _resolution;
            math::Point3D _position;
            math::Vector3D _rotation;
            uint _fov = DEFAULT_FIELD_OF_VIEW;
    };
};// namespace raytracer
// clang-format on
