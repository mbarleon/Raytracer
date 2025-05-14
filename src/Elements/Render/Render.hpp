/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "Config/Antialiasing.hpp"
#include "Config/Lighting.hpp"
#include "Config/Output.hpp"

// clang-format off
namespace raytracer {
struct RenderConfig {
	Antialiasing antialiasing;
	unsigned int maxDepth;
	Lighting lighting;
	RenderOutput output;
};
};// namespace raytracer
// clang-format on
