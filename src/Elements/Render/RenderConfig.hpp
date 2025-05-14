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
#include "../../Maths/Vector3D.hpp"

// clang-format off
namespace raytracer {
struct RenderConfig {
	Antialiasing antialiasing;
	math::RGBColor background;
	Lighting lighting;
	unsigned int maxDepth;
	RenderOutput output;
};
};// namespace raytracer
// clang-format on
