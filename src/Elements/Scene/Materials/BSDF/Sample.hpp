/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sample
*/

#pragma once

#include "../../../../Maths/Vector3D.hpp"

namespace raytracer::material {
struct BSDFSample {
    math::Vector3D direction;
    double pdf;
    math::RGBColor value;
};
};
