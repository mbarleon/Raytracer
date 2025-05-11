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
#include "../../Scene/Materials/Material.hpp"
#include "../../Scene/Shapes/IShape.hpp"
#include "../../Scene/Lights/ILight.hpp"
#include "Macro.hpp"
#include <vector>

namespace raytracer {

using IShapesList = std::vector<std::shared_ptr<raytracer::shape::IShape>>;
using ILightsList = std::vector<std::shared_ptr<raytracer::light::ILight>>;

// Lighting
math::RGBColor computeDirectLight(const math::Intersect &intersect,
    const math::Vector3D &V, const material::Material &mat, const IShapesList &shapes,
    const ILightsList &lights, const Render &render);

// Color
math::RGBColor getBackgroundColor(const math::Vector3D &v);
math::RGBColor getRayColor(const math::Ray &ray, const IShapesList &shapes,
    const ILightsList &lights, const Render &render, unsigned depth);

// Intersect
bool findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
    math::Intersect &intersect, const bool cullBackFaces);

};
