/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.hpp
*/

#pragma once

#include "../../Maths/Intersect.hpp"
#include "../../Maths/Ray.hpp"
#include "../../Maths/Vector2u.hpp"
#include "../../Maths/Vector3D.hpp"
#include "../Render/Render.hpp"
#include "../Scene/Shapes/IShape.hpp"
#include "../Render/Structs/ReSTIR_Tank.hpp"
#include "ImagePixel.hpp"
#include "Macro.hpp"
#include <memory>
#include <vector>

// clang-format off
using IShapesList = std::vector<std::shared_ptr<raytracer::shape::IShape>>;

namespace raytracer {
    class Camera final
    {
        public:
            explicit Camera(const math::Vector2u &resolution, const math::Point3D &position, const math::Vector3D &rotation, const uint field_of_view);
            ~Camera() = default;

            void generateRay(double u, double v, math::Ray &cameraRay) const noexcept;
            void render(const IShapesList &shapes, const IShapesList &lights,
                const Render &render) const;

        private:
            math::Vector2u _resolution;
            math::Point3D _position;
            math::Vector3D _rotation;
            uint _fov = DEFAULT_FIELD_OF_VIEW;
    };

    bool findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
        const IShapesList &lights, math::Intersect &intersect, bool cullBackFaces);
    const RGBColor traceRay(const ImagePixel &pixel, const math::Ray &ray,
        const IShapesList &shapes, const IShapesList &lights, unsigned int depth,
        const Render &render, bool cullBackFaces, std::vector<std::vector<ReSTIR_Tank>> &tank_grid);
    const RGBColor computeDirectLighting(const ImagePixel &pixel,
        const math::Ray &ray, const math::Intersect &intersect, const IShapesList &shapes,
        const IShapesList &lights, const Render &render,
        std::vector<std::vector<ReSTIR_Tank>> &tank_grid);
    const RGBColor computeAmbientOcclusion(const math::Intersect &intersect,
        unsigned int aoSamples, const IShapesList &shapes, const IShapesList &lights);
    const RGBColor computeRefraction(const ImagePixel &pixel,
        const math::Ray &incoming, const math::Intersect &intersect, const IShapesList &shapes,
        const IShapesList &lights, unsigned int depth, const Render &render,
        std::vector<std::vector<ReSTIR_Tank>> &tank_grid);
    const RGBColor computeReflection(const ImagePixel &pixel,
        const math::Ray &incoming, const math::Intersect &intersect, const IShapesList &shapes,
        const IShapesList &lights, unsigned int depth, const Render &render,
        std::vector<std::vector<ReSTIR_Tank>> &tank_grid);

    static inline const math::Vector3D reflect(const math::Vector3D &I, const math::Vector3D &N)
    {
        return I - N * (2.0 * I.dot(N));
    }

    static inline math::Ray offsetRay(const math::Vector3D &origin,const math::Vector3D &normal,
        const math::Vector3D &direction)
    {
        const math::Vector3D offset = (direction.dot(normal) > 0) ? normal : -normal;
        return {origin + offset * EPSILON, direction};
    }
};// namespace raytracer
// clang-format on
