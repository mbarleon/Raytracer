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
#include "../../Render/ReSTIR/Tank.hpp"
#include "Macro.hpp"
#include <vector>

namespace raytracer {

using IShapesList = std::vector<std::shared_ptr<raytracer::shape::IShape>>;
using ILightsList = std::vector<std::shared_ptr<raytracer::light::ILight>>;

// Lighting
math::RGBColor phongDirect(const math::Intersect &isect, const math::Vector3D &viewDir,
    const ILightsList &lights, const IShapesList &shapes, const Render &render,
    std::mt19937 &rng);
double ambientOcclusion(const math::Intersect &isect, const IShapesList &shapes,
    int aoSamples, std::mt19937 &rng);

// Color
math::RGBColor getBackgroundColor(const math::Vector3D &v, const math::RGBColor &background);
LightSample getRayColor(const math::Ray &ray, const IShapesList &shapes,
    const ILightsList &lights, const Render &render, unsigned depth,
    std::mt19937 &rng, const math::RGBColor &throughput = math::RGBColor(1.0));

// Intersect
bool findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
    math::Intersect &intersect, const bool cullBackFaces);

};
