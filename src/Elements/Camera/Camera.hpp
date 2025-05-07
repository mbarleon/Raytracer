/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.hpp
*/

#pragma once

#include <vector>
#include <memory>
#include "../../Maths/Vector2u.hpp"
#include "../../Maths/Vector3D.hpp"
#include "../../Maths/Ray.hpp"
#include "../../Maths/Intersect.hpp"
#include "../Scene/Shapes/IShape.hpp"
#include "../Render/Render.hpp"
#include "Macro.hpp"

// clang-format off
using IShapesList = std::vector<std::shared_ptr<raytracer::shape::IShape>>;

namespace raytracer {
    class Camera final
    {
        public:
            explicit Camera(const math::Vector2u &resolution, const math::Point3D &position, const math::Vector3D &rotation, const uint field_of_view);
            ~Camera() = default;

            void generateRay(double u, double v, math::Ray &cameraRay) const noexcept;
            void render(const IShapesList &shapes, const Render &render) const;

        private:
            math::Vector2u _resolution;
            math::Point3D _position;
            math::Vector3D _rotation;
            uint _fov = DEFAULT_FIELD_OF_VIEW;
    };

    bool findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
        math::Intersect &intersect);
    RGBColor traceRay(const math::Ray &ray, const IShapesList &shapes,
        unsigned int depth, const Render &render);
    RGBColor computeColor(const math::Intersect &intersect, const math::Ray &ray,
        const IShapesList & shapes, unsigned int depth, const Render &render);
    RGBColor computeLighting(const math::Point3D &P, const math::Vector3D &N,
        const math::Vector3D &V, const RGBColor &surfaceColor, const Material &M,
        const IShapesList &shapes, const Render &render);
    inline math::Vector3D reflect(const math::Vector3D &I, const math::Vector3D &N);
    RGBColor computeRefraction(const math::Ray &ray, const math::Intersect &intersect,
        const IShapesList &shapes, unsigned int depth, const Render &render);
    RGBColor computeReflection(const math::Ray &ray, const math::Intersect &intersect,
        const IShapesList &shapes, unsigned int depth, const Render &render);
};// namespace raytracer
// clang-format on
