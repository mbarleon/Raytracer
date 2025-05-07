/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#pragma once

// clang-format off
namespace raytracer {
typedef struct material {
    double reflectivity;
    double transparency;
    double refractiveIndex;
    double emissiveIntensity;
    double shininess;
} Material;
};// namespace raytracer
// clang-format on
