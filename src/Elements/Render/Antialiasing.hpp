/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include <string>

// clang-format off
namespace raytracer {
typedef struct antialiasing {
    std::string type;
    unsigned int samples;
} Antialiasing;
};// namespace raytracer
// clang-format on
