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
//using ILightsList = std::vector<std::shared_ptr<raytracer::shape::ILight>>;

math::RGBColor getBackgroundColor(const math::Vector3D &v);

bool findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
    math::Intersect &intersect, const bool cullBackFaces);

};
