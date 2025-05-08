/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ReSTIR
*/

#pragma once

// clang-format off
namespace raytracer {
struct spatial_restir {
    double radius;
    unsigned int samples;
};

struct temporal_restir {
    unsigned int samples;
};

typedef struct restir {
    struct spatial_restir spatial;
    struct temporal_restir temporal;
} ReSTIR;
};// namespace raytracer
// clang-format on
