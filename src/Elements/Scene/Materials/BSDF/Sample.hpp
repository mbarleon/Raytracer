/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sample
*/

#pragma once

#include "../../../../Maths/Vector3D.hpp"

namespace raytracer::material {

// clang-format off
struct BSDFSample {
    math::Vector3D direction;
    double pdf;
    math::RGBColor radiance;
    bool isDelta = false;
};
// clang-format on
}// namespace raytracer::material
