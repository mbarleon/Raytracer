/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Pathtracer
*/

#pragma once

#include "../../../Maths/Vector3D.hpp"
#include "../../../Maths/Intersect.hpp"
#include "../../../Maths/Ray.hpp"
#include "../../Render/Render.hpp"
#include "../../Scene/Material.hpp"
#include "../../Scene/Shapes/IShape.hpp"
#include "../../Scene/Lights/ILight.hpp"
#include "Macro.hpp"
#include <vector>

namespace raytracer {

using IShapesList = std::vector<std::shared_ptr<raytracer::shape::IShape>>;
// using ILightsList = std::vector<std::shared_ptr<raytracer::shape::ILight>>;

// Reflect
math::Vector3D getReflectedVector(const math::Vector3D &I, const math::Vector3D &N);
math::Vector3D getRandomReflectUnitSphere();
math::Vector3D reflectHemisphere(const math::Vector3D &normal);

// Color
math::RGBColor getBackgroundColor(const math::Vector3D &v);
math::RGBColor getRayColor(const math::Ray &ray, const IShapesList &shapes,
    const Render &render, unsigned depth);

// Intersect
bool findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
    math::Intersect &intersect, const bool cullBackFaces);

// Random
double getRandomDouble();
double getRandomDouble(double min, double max);
math::Vector3D getRandomVector();
math::Vector3D getRandomVector(double min, double max);

};
