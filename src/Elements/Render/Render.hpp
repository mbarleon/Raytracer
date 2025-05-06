/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "../Scene/Material/Color.hpp"
#include "Antialiasing.hpp"
#include "Output.hpp"

// clang-format off
namespace raytracer {
typedef struct render {
	RGBColor background;
	Antialiasing antialiasing;
	RenderOutput output;
	unsigned int maxDepth;
} Render;
};// namespace raytracer
// clang-format on
