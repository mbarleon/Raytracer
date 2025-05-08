/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "Config/Antialiasing.hpp"
#include "Config/Output.hpp"
#include "Config/Lighting.hpp"
#include "Config/AmbiantOcclusion.hpp"

// clang-format off
namespace raytracer {
typedef struct render {
	Antialiasing antialiasing;
	AmbientOcclusion occlusion;
	unsigned int maxDepth;
	Lighting lighting;
	RenderOutput output;
} Render;
};// namespace raytracer
// clang-format on
