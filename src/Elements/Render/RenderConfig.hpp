/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "../../Maths/Vector3D.hpp"
#include "Config/Antialiasing.hpp"
#include "Config/Lighting.hpp"
#include "Config/Output.hpp"
#include "../Scene/Textures/ITexture.hpp"

// clang-format off
namespace raytracer {
struct RenderConfig {
	Antialiasing antialiasing;
	std::shared_ptr<texture::ITexture> skybox = nullptr;
	Lighting lighting;
	unsigned int maxDepth;
	RenderOutput output;
};
}// namespace raytracer
// clang-format on
