/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbiantLight
*/

#pragma once

#include "Lighting/AmbientOcclusion.hpp"

// clang-format off
namespace raytracer {
struct Lighting {
    double gamma;
    AmbientOcclusion ambient;
    double diffuse;
    double specular;
};
};// namespace raytracer
// clang-format on
