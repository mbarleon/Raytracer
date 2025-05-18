/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "../Scene/Textures/ITexture.hpp"
#include "Config/Antialiasing.hpp"
#include "Config/Lighting.hpp"
#include "Config/Output.hpp"
#include <memory>

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
