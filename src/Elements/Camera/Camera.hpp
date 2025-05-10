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

    const RGBColor computeAmbientOcclusion(const math::Intersect &intersect,
        unsigned int aoSamples, const IShapesList &shapes, const IShapesList &lights,
        std::mt19937 &rng);
    LightSample sampleDirectLight(const math::Ray &incoming, const math::Intersect &intersect,
        const IShapesList &shapes, const IShapesList &lights, const Render &render,
        std::mt19937 &rng);
    bool findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
        const IShapesList &lights, math::Intersect &intersect, bool cullBackFaces);

    static inline const math::Vector3D reflect(const math::Vector3D &I, const math::Vector3D &N)
    {
        return I - N * (2.0 * I.dot(N));
    }

    static inline math::Ray offsetRay(const math::Vector3D &origin, const math::Vector3D &normal,
        const math::Vector3D &direction)
    {
        const math::Vector3D offset = (direction.dot(normal) > 0) ? normal : -normal;
        return {origin + offset * EPSILON, direction};
    }

    static inline math::Vector3D sampleHemisphereDirectionFromShape(const shape::IShape &shape,
        const math::Vector3D &origin, const math::Vector3D &normal, std::mt19937 &rng)
    {
        const math::Point3D sample = shape.getRandomPointOnSurface(rng);
        math::Vector3D dir = (sample - origin).normalize();

        if (dir.dot(normal) < 0.0) {
            dir = -dir;
        }
        return dir;
    }

    static inline math::Vector3D cosineSampleHemisphere(double u1, double u2)
    {
        const double r = std::sqrt(1.0 - u1 * u1);
        const double phi = 2.0 * M_PI * u2;
        return math::Vector3D(
            r * std::cos(phi),
            r * std::sin(phi),
            u1
        );
    }

    static inline void buildOrthonormalBasis(const math::Vector3D &n, math::Vector3D &t, math::Vector3D &b)
    {
        if (std::fabs(n._x) > std::fabs(n._z)) {
            t = math::Vector3D(-n._y, n._x, 0.0).normalize();
        } else {
            t = math::Vector3D(0.0, -n._z, n._y).normalize();
        }
        b = n.cross(t);
    }
};// namespace raytracer
// clang-format on
