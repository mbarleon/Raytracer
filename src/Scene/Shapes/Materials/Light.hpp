/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Light.hpp
*/

#pragma once

#include "AMaterial.hpp"
namespace raytracer::material {
class Light final : public AMaterial
{
    public:
        Light() = default;
        ~Light() override = default;
};
}// namespace raytracer::material
