/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle.hpp
*/

#pragma once

#include "../../../../Maths/Vector3D.hpp"
#include "../AShape.hpp"

namespace raytracer::shape {

/**
* @class Triangle
* @brief represents a Triangle shape
*/
class Triangle : public AShape
{
    public:
        /**
         * @brief construct a Triangle with vertices and optional UV coords
         * @param p0,p1,p2 Triangle vertices
         */
        explicit Triangle(const math::Point3D &p0, const math::Point3D &p1, const math::Point3D &p2) noexcept;

        math::Vector3D getPosition() const override;
        math::Vector3D getNormalAt(const math::Point3D __attribute__((unused)) &) const noexcept override;
        double getAOMaxDistance() const override;
        void getUV(const math::Point3D &p, double &u, double &v) const noexcept override;
        math::RGBColor getColorAt(const math::Point3D &p) const override;
        bool intersect(const math::Ray &ray, math::Point3D &intPoint, const bool cullBackFaces) const noexcept override;

    private:
        void init(void) noexcept;

        math::Point3D _p0, _p1, _p2;
        math::Vector3D _edge1, _edge2;
        math::Vector3D _normal;
        math::Point3D _centroid;
        double _aomax;
};
}// namespace raytracer::shape
