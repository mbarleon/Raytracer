/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "Antialiasing.hpp"
#include "Output.hpp"
#include "AmbiantLight.hpp"

// clang-format off
namespace raytracer {
typedef struct render {
	Antialiasing antialiasing;
	AmbiantLight ambiantLight;
	unsigned int maxDepth;
	RenderOutput output;
} Render;
};// namespace raytracer
// clang-format on
