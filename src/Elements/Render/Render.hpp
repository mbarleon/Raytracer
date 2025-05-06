/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "Antialiasing.hpp"
#include "Output.hpp"
#include "Lighting.hpp"

// clang-format off
namespace raytracer {
typedef struct render {
	Antialiasing antialiasing;
	RGBColor background;
	Lighting lighting;
	unsigned int maxDepth;
	RenderOutput output;
} Render;
};// namespace raytracer
// clang-format on
