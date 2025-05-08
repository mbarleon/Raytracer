/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbiantLight
*/

#pragma once

// clang-format off
namespace raytracer {
typedef struct lighting {
    double ambient;
    double diffuse;
    double specular;
} Lighting;
};// namespace raytracer
// clang-format on
