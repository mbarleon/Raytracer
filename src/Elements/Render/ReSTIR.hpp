/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ReSTIR
*/

#include "ReSTIR/Spatial.hpp"
#include "ReSTIR/Temporal.hpp"

#pragma once

// clang-format off
namespace raytracer {
typedef struct restir {
    SpacialReSTIR spacial;
    TemporalReSTIR temporal;
} ReSTIR;
};// namespace raytracer
// clang-format on
