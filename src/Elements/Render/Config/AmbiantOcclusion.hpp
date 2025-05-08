/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "AmbiantOcclusion/ReSTIR.hpp"

// clang-format off
namespace raytracer {
typedef struct ambient_occlusion {
    unsigned int samples;
    ReSTIR restir;
} AmbientOcclusion;
};// namespace raytracer
// clang-format on
