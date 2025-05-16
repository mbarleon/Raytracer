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
struct Antialiasing {
    unsigned int samples;
    std::string mode;
};
}// namespace raytracer
// clang-format on
