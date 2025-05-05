/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Intersect
*/

#pragma once
#include "../Elements/Scene/Shapes/IShape.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

namespace math {
    typedef struct Intersect {
        std::shared_ptr<raytracer::shape::IShape> object;
        Point3D point;
        Vector3D normal; // normale au point
        double distance;
    } Intersect;
};
