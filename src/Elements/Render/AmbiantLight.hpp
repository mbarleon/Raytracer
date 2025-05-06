/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbiantLight
*/

#pragma once

#include "../Scene/Material/Color.hpp"

// clang-format off
namespace raytracer {
typedef struct ambiant_light {
    RGBColor color;
    double intensity;
} AmbiantLight;
};// namespace raytracer
// clang-format on
